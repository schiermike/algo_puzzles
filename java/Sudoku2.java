

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.HashSet;

public class Sudoku2 
{
	private static class SolvedException extends RuntimeException 
	{
		private static final long serialVersionUID = 1L;
	};
	
	private static int[][] oldS = new int[9][9];
	private static int[][] oldSRotated;
	private static int[][] newS  = new int[9][9];
	
	public static void main(String[] args) throws IOException 
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		//BufferedReader reader = new BufferedReader(new FileReader("testcases/SUDOKU2_1.txt"));
		int numInputs = Integer.parseInt(reader.readLine());
	
		// iterate over each problem
		while(numInputs-->0)
		{
			StringBuffer sudokuString1 = new StringBuffer();
			for(int i=0; i<9; i++)
				sudokuString1.append(reader.readLine());

			StringBuffer sudokuString2 = new StringBuffer();
			for(int i=0; i<9; i++)
				sudokuString2.append(reader.readLine());

			boolean result = processProblem(new Sudoku(sudokuString1.toString()), new Sudoku(sudokuString2.toString()));
			System.out.println(result ? "Yes" : "No");
			
			reader.readLine(); // skip empty line
		}
	}

	private static boolean processProblem(Sudoku oldSudoku, Sudoku newSudoku) 
	{
		SudokuSolver solver = new SudokuSolver(newSudoku);
		solver.setMaxConcurrentGuesses(1);
		if(SudokuResult.SOLVED != solver.solve()) // this should not happen
			return false;
		
		for(int row = 0; row < 9; row++)
			for(int col = 0; col < 9; col++)
			{
				oldS[row][col] = oldSudoku.getEntry(row, col);
				newS[row][col] = newSudoku.getEntry(row, col);
			}
		oldSRotated = rotateCW(oldS);

		// solving process is recursively nested - a solved exception indicates success
		try
		{
			switchRowSegmentRound();
			return false;
		}
		catch(SolvedException e)
		{
			return true;
		}
	}
	
	private static void switchRowSegmentRound()
	{
		// iterate over all possible permutations (6)
		for(int step1 = 0; step1 < 3; step1++)
			for(int step2 = 1; step2 < 3; step2++)
			{
				// do row segment switching
				switchRowSegments(0, step1);
				switchRowSegments(1, step2);
				
				switchRowRound();
				
				// switch row segments back
				switchRowSegments(1, step2);
				switchRowSegments(0, step1);
			}
	}
	
	private static void switchRowRound() 
	{
		// iterate over all possible permutations of segment 1
		for(int step11 = 0; step11 < 3; step11++)
			for(int step12 = 1; step12 < 3; step12++)
			{
				// iterate over all possible permutations of segment 2
				for(int step21 = 3; step21 < 6; step21++)
					for(int step22 = 4; step22 < 6; step22++)
					{
						// iterate over all possible permutations of segment 3
						for(int step31 = 6; step31 < 9; step31++)
							for(int step32 = 7; step32 < 9; step32++)
							{
								// do row switching
								switchRows(0, step11);
								switchRows(1, step12);
								switchRows(3, step21);
								switchRows(4, step22);
								switchRows(6, step31);
								switchRows(7, step32);
								
								checkRound(oldS);
								checkRound(oldSRotated);
								
								// switch row segments back
								switchRows(7, step32);
								switchRows(6, step31);
								switchRows(4, step22);
								switchRows(3, step21);
								switchRows(1, step12);
								switchRows(0, step11);
							}
					}
			}
	}
	

	
	/**
	 * let's see whether we can find a fitting column mapping
	 */
	private static int[] mapping = new int[9];
	private static void checkRound(int[][] otherS) 
	{
		for(int targetCol = 0; targetCol < 9; targetCol++)
		{
			// with the first column of otherS, establish the mapping to the targetCol
			for(int row = 0; row < 9; row++)
				mapping[otherS[row][0]-1] = newS[row][targetCol];
			
			// now check whether we can find matches for all other columns
			boolean colMatchTest = false;
			for(int sCol = 1; sCol < 9; sCol++)
			{
				for(int tCol=0; tCol<9; tCol++)
				{
					if(tCol == targetCol)
						continue;
					
					colMatchTest = true;
					for(int row = 0; row < 9; row++)
					{
						if(mapping[otherS[row][sCol]-1] != newS[row][tCol])
						{
							// columns do not match - try the next targetColumn
							colMatchTest = false;
							break;
						}
					}
					// columns matched - the current sourceColumn matches thsi targetColumn
					if(colMatchTest)
						break;
				}
				// we couldn't associate the sourceColumn with a targetColumn
				if(!colMatchTest)
					break;
			}
			if(colMatchTest)
				throw new SolvedException();
		}
	}
	
	/**
	 * clock-wise rotation without allocating new space 
	 */
	private static int[][] rotateCW(int[][] s)
	{
		int[][] s2 = new int[9][9];
		for(int row = 0; row < 9; row++)
			for(int col = 0; col < 9; col++)
				s2[col][8-row] = s[row][col];
		return s2;
	}
	
	/**
	 * necessary function SR(s,r1,r2)
	 */
	private static void switchRows(int row1, int row2)
	{
		if(row1 == row2)
			return;
		
		int[] temp = newS[row1];
		newS[row1] = newS[row2];
		newS[row2] = temp;
	}
	
	/**
	 * necessary function SRS(s,rs1,rs2)  
	 */
	private static void switchRowSegments(int segRow1, int segRow2)
	{
		if(segRow1 == segRow2)
			return;
		
		segRow1 *= 3;
		segRow2 *= 3;
		
		int[] temp = newS[segRow1];
		newS[segRow1] = newS[segRow2];
		newS[segRow2] = temp;
		temp = newS[segRow1+1];
		newS[segRow1+1] = newS[segRow2+1];
		newS[segRow2+1] = temp;
		temp = newS[segRow1+2];
		newS[segRow1+2] = newS[segRow2+2];
		newS[segRow2+2] = temp;
	}
	
	private static class Sudoku
	{
		private HashMap<Integer, HashSet<Integer>> possibilities = new HashMap<Integer, HashSet<Integer>>();
		
		public HashSet<Integer> getPossibilities(int row, int col)
		{
			return possibilities.get(9 * row + col);
		}
		
		public boolean isSolved()
		{
			return countFixed() == 81;
		}
		
		public int getEntry(int row, int col)
		{
			return possibilities.get(9 * row + col).iterator().next();
		}
		
		public void setEntry(int row, int col, int entry)
		{
			HashSet<Integer> p = possibilities.get(9 * row + col);
			if(!p.contains(entry))
				throw new RuntimeException("Try to set an element which is not possible!");
			p.clear();
			p.add(entry);
		}
		
		public boolean isFixed(int row, int col)
		{
			return possibilities.get(9 * row + col).size() == 1;
		}
		
		public Sudoku(Sudoku other)
		{
			other.copyTo(this);
		}
		
		public Sudoku() 
		{
			HashSet<Integer> allPossibilities = new HashSet<Integer>();
			for(int entry = 1; entry <= 9; entry++)
				allPossibilities.add(entry);
			
			for(int row = 0; row < 9; row++)
			{
				for(int col = 0; col < 9; col++)
				{
					HashSet<Integer> set = new HashSet<Integer>();
					set.addAll(allPossibilities);
					possibilities.put(9 * row + col, set);
				}
			}
		}
		
		public Sudoku(String sudokuString)
		{
			this();
			if(sudokuString == null || sudokuString.length() != 81)
				throw new IllegalArgumentException("Sudoku-String has to have a length of 81!");
			
			for(int i=0; i < 81; i++)
			{
				char c = sudokuString.charAt(i);
				if(c >= '1' && c <= '9')
					setEntry(i / 9, i % 9, c-'0');
			}
		}
		
		@Override
		public String toString() 
		{
			String out = "";
			for(int row = 0; row < 9; row++)
			{
				for(int col = 0; col < 9; col++)
				{
					
					out += isFixed(row, col) ? getEntry(row, col) : "-";
					out += (col<8 && (col+1)%3==0) ? " | " : " ";
				}
				out += (row<8 && (row+1)%3==0) ? "\n=====================\n" : "\n";
			}
			return out;
		}
		
		@SuppressWarnings("unchecked")
		public void copyTo(Sudoku target) 
		{
			target.possibilities.clear();
			for(Integer i : this.possibilities.keySet())
				target.possibilities.put(i, (HashSet<Integer>)this.possibilities.get(i).clone());
		}
	
		public int countFixed() 
		{
			int numFixed = 0;
			for(int row = 0; row < 9; row++)
				for(int col = 0; col < 9; col++)
					if(isFixed(row, col))
						numFixed++;
			return numFixed;
		}
	}

	private static enum SudokuResult
	{
		SOLVED,
		STUCK,
		IMPOSSIBLE;
		
		public String toString()
		{
			switch(this)
			{
				case SOLVED: return "SOLVED";
				case STUCK: return "STUCK";
				case IMPOSSIBLE: return "IMPOSSIBLE";
				default: return "";
			}
		};
	}

	private static class SudokuSolver 
	{
		private static class ImpossibleBranchException extends IllegalStateException
		{
			private static final long serialVersionUID = 1L;
		}
		
		private final Sudoku sudoku;
		private int maxConcurrentGuesses = 1;
		
		public SudokuSolver(Sudoku sudoku)
		{
			this.sudoku = sudoku;
		}
		
		public SudokuResult solve()
		{
			return solve(sudoku, maxConcurrentGuesses);
		}
		
		/**
		 * recursive solving function, cascades with tryAndError
		 * recursionDepth is equal to the number of concurrently made guesses
		 * @param sudoku
		 * @param recursionDepth
		 * @param node
		 * @return
		 */
		private SudokuResult solve(Sudoku sudoku, int maxGuesses)
		{
			while(true)
			{
				try 
				{
					simplify(sudoku);
				} 
				catch (ImpossibleBranchException e) 
				{
					return SudokuResult.IMPOSSIBLE;
				}
				
				if(sudoku.isSolved())
					return SudokuResult.SOLVED;
		
				/**
				 *  if we are already guessing - bad luck
				 *  
				 *  if we are not guessing, so we can start with it now
				 *  if we have no luck with that, we got stuck in this branch
				 *  else we go on to another loop
				 */
				if(maxGuesses <= 0)
					return SudokuResult.STUCK;
	
				if(!tryAndError(sudoku, maxGuesses))
					return SudokuResult.STUCK;
			}
		}
		
		/**
		 * try to simplify the sudoku as far as possible by removing impossible entries
		 * @param sudoku
		 * @param node
		 */
		private void simplify(Sudoku sudoku) 
		{
			boolean progress;
			do
			{
				progress = removeImpossibles(sudoku);
				progress |= findUniquePossibilities(sudoku);
				progress |= findLineFieldMatches(sudoku);
			}
			while(progress);
		}
		
		/**
		 * remove possibilities from row, column and square
		 */
		private boolean removeImpossibles(Sudoku sudoku)
		{
			boolean progress = false;
			
			for(int row = 0; row < 9; row++)
			{
				for(int col = 0; col < 9; col++)
				{
					if(!sudoku.isFixed(row, col))
						continue;
					int entry = sudoku.getEntry(row, col);
					
					// check for each row
					for(int row1 = 0; row1 < 9; row1++)
					{
						if(row == row1)
							continue;
						progress |= canRemoveEntry(entry, sudoku, row1, col);
					}
					
					// check for each column
					for(int col1 = 0; col1 < 9; col1++)
					{
						if(col == col1)
							continue;
						progress |= canRemoveEntry(entry, sudoku, row, col1);
					}
					
					// check for each field
					for(int row1 = (row/3)*3; row1 < (row/3)*3 + 3; row1++)
					{
						for(int col1 = (col/3)*3; col1 < (col/3)*3 + 3; col1++)
						{
							if(row == row1 && col == col1)
								continue;
							progress |= canRemoveEntry(entry, sudoku, row1, col1);
						}
					}
				}
			}
			return progress;
		}
		
		/**
		 * find unique possibilites in row, column and field
		 * example: -> deduce 3 if 3 in x
		 * x74......
		 * ....3....
		 * ........3
		 */
		private boolean findUniquePossibilities(Sudoku sudoku)
		{
			boolean progress = false;
			for(int row = 0; row < 9; row++)
			{
				for(int col = 0; col < 9; col++)
				{
					if(sudoku.isFixed(row, col))
						continue;
					HashSet<Integer> p = sudoku.getPossibilities(row, col);
					
					// check for each row whether one element is left
					HashSet<Integer> pRow = new HashSet<Integer>(p);
					for(int row1 = 0; row1 < 9; row1++)
					{
						if(row == row1)
							continue;
						pRow.removeAll(sudoku.getPossibilities(row1, col));
					}
					if(pRow.size() == 1)
					{
						p.clear();
						p.addAll(pRow);
						progress = true;
						continue;
					}
					
					// check for each column whether one element is left
					HashSet<Integer> pCol = new HashSet<Integer>(p);
					for(int col1 = 0; col1 < 9; col1++)
					{
						if(col == col1)
							continue;
						pCol.removeAll(sudoku.getPossibilities(row, col1));
					}
					if(pCol.size() == 1)
					{
						p.clear();
						p.addAll(pCol);
						progress = true;
						continue;
					}
					
					// check for each field whether one element is left
					HashSet<Integer> pField = new HashSet<Integer>(p);
					for(int row1 = (row/3)*3; row1 < (row/3)*3 + 3; row1++)
					{
						for(int col1 = (col/3)*3; col1 < (col/3)*3 + 3; col1++)
						{
							if(row == row1 && col == col1)
								continue;
							pField.removeAll(sudoku.getPossibilities(row1, col1));
						}
					}
					if(pField.size() == 1)
					{
						p.clear();
						p.addAll(pField);
						progress = true;
						continue;
					}
				}
			}
			return progress;
		}
		
		/**
		 * Check whether we can deduce the following exemplary information:
		 * in field 3, x is only possible in row 2, therefore no x is possible in row 2, field 1
		 * -xx|---|---    -xx|---|---
		 * x--|---|x-x -> ---|---|x-x
		 * ---|x--|---    ---|x--|---
		 * @param sudoku
		 * @param node
		 * @return
		 */
		private boolean findLineFieldMatches(Sudoku sudoku)
		{
			boolean progress = false;
			
			for(int row = 0; row < 9; row += 3)
			{
				for(int col = 0; col < 9; col += 3)
				{
					// row checking
					HashSet<Integer> pRow1 = new HashSet<Integer>(sudoku.getPossibilities(row, col));
					pRow1.addAll(sudoku.getPossibilities(row, col+1));
					pRow1.addAll(sudoku.getPossibilities(row, col+2));
					HashSet<Integer> pRow2 = new HashSet<Integer>(sudoku.getPossibilities(row+1, col));
					pRow2.addAll(sudoku.getPossibilities(row+1, col+1));
					pRow2.addAll(sudoku.getPossibilities(row+1, col+2));
					HashSet<Integer> pRow3 = new HashSet<Integer>(sudoku.getPossibilities(row+2, col));
					pRow3.addAll(sudoku.getPossibilities(row+2, col+1));
					pRow3.addAll(sudoku.getPossibilities(row+2, col+2));
					
					HashSet<Integer> pOnlyThisRow = new HashSet<Integer>(pRow1);
					pOnlyThisRow.removeAll(pRow2);
					pOnlyThisRow.removeAll(pRow3);
					if(!pOnlyThisRow.isEmpty())
						for(int c = 0; c < 6; c++)
							sudoku.getPossibilities(row, (col+3+c)%9).removeAll(pOnlyThisRow);
					
					pOnlyThisRow = new HashSet<Integer>(pRow2);
					pOnlyThisRow.removeAll(pRow1);
					pOnlyThisRow.removeAll(pRow3);
					if(!pOnlyThisRow.isEmpty())
						for(int c = 0; c < 6; c++)
							sudoku.getPossibilities(row+1, (col+3+c)%9).removeAll(pOnlyThisRow);
					
					pOnlyThisRow = new HashSet<Integer>(pRow3);
					pOnlyThisRow.removeAll(pRow1);
					pOnlyThisRow.removeAll(pRow2);
					if(!pOnlyThisRow.isEmpty())
						for(int c = 0; c < 6; c++)
							sudoku.getPossibilities(row+2, (col+3+c)%9).removeAll(pOnlyThisRow);
					
					// --------------------------------------------------------------------------
					
					// column checking
					HashSet<Integer> pCol1 = new HashSet<Integer>(sudoku.getPossibilities(row, col));
					pCol1.addAll(sudoku.getPossibilities(row+1, col));
					pCol1.addAll(sudoku.getPossibilities(row+2, col));
					HashSet<Integer> pCol2 = new HashSet<Integer>(sudoku.getPossibilities(row, col+1));
					pCol2.addAll(sudoku.getPossibilities(row+1, col+1));
					pCol2.addAll(sudoku.getPossibilities(row+2, col+1));
					HashSet<Integer> pCol3 = new HashSet<Integer>(sudoku.getPossibilities(row, col+2));
					pCol3.addAll(sudoku.getPossibilities(row+1, col+2));
					pCol3.addAll(sudoku.getPossibilities(row+2, col+2));
					
					HashSet<Integer> pOnlyThisCol = new HashSet<Integer>(pCol1);
					pOnlyThisCol.removeAll(pCol2);
					pOnlyThisCol.removeAll(pCol3);
					if(!pOnlyThisCol.isEmpty())
						for(int c = 0; c < 6; c++)
							sudoku.getPossibilities((row+3+c)%9, col).removeAll(pOnlyThisCol);
					
					pOnlyThisCol = new HashSet<Integer>(pCol2);
					pOnlyThisCol.removeAll(pCol1);
					pOnlyThisCol.removeAll(pCol3);
					if(!pOnlyThisCol.isEmpty())
						for(int c = 0; c < 6; c++)
							sudoku.getPossibilities((row+3+c)%9, col+1).removeAll(pOnlyThisCol);
					
					pOnlyThisCol = new HashSet<Integer>(pCol3);
					pOnlyThisCol.removeAll(pCol1);
					pOnlyThisCol.removeAll(pCol2);
					if(!pOnlyThisCol.isEmpty())
						for(int c = 0; c < 6; c++)
							sudoku.getPossibilities((row+3+c)%9, col+2).removeAll(pOnlyThisCol);
				}
			}
			
			return progress;
		}
	
		/**
		 * check whether entry is possible for the cell (row,col) and if yes, remove the possibility
		 * if the set of possibilities gets empty, we are stuck
		 * @param entry
		 * @param sudoku
		 * @param row
		 * @param col
		 * @param node
		 * @throws ImpossibleBranchException
		 * @return true if we can remove an entry and don't get stuck
		 */
		private boolean canRemoveEntry(int entry, Sudoku sudoku, int row, int col) throws ImpossibleBranchException 
		{
			HashSet<Integer> p = sudoku.getPossibilities(row, col);
			if(!p.contains(entry))
				return false;
			
			p.remove(entry);
			
			if(!p.isEmpty())
				return true;
			
			throw new ImpossibleBranchException();
		}
	
		private boolean tryAndError(Sudoku sudoku, int maxGuesses)
		{
			// step 3: try to guess entry starting with the smallest possibilities sets
			for(int setSize = 2; setSize <= 9; setSize++)
			{
				for(int row = 0; row < 9; row++)
				{
					for(int col = 0; col < 9; col++)
					{
						// ignore filled fields
						if(sudoku.isFixed(row, col))
							continue;
						// ignore unfilled fields with an inappropriate set size
						if(sudoku.getPossibilities(row, col).size() != setSize)
							continue;
						
						// try all possible candidates
						for(int candidate : sudoku.getPossibilities(row, col))
						{
							Sudoku sudokuClone = new Sudoku(sudoku);
							sudokuClone.setEntry(row, col, candidate);
							// now try to solve this simplified sudoku
							
							// IMPORTANT TUNING PART: the more possibilities are left, the fewer recursions are allowed
							//int newMaxGuesses = maxGuesses + 1 - setSize;
							int newMaxGuesses = maxGuesses - 1;
							
							SudokuResult result = solve(sudokuClone, newMaxGuesses);
							switch(result)
							{
								case SOLVED:
									// we were lucky and could solve the entire sudoku!
									sudokuClone.copyTo(sudoku);
									return true;
								case IMPOSSIBLE:
									// the choice turned out to be impossible, so we can exclude this possibility
									sudoku.getPossibilities(row, col).remove(candidate);
									return true;
								case STUCK:
									// we had no luck and just try the next candidate
									break;
							}
						}
					}
				}
			}
			
			return false;
		}
	
		public void setMaxConcurrentGuesses(int maxConcurrentGuesses) 
		{
			this.maxConcurrentGuesses = maxConcurrentGuesses;
		}
	}
}

