import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;


public class Typeset 
{
	public static void main(String[] args) throws IOException 
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		//BufferedReader reader = new BufferedReader(new FileReader("testcases/TYPESET.txt"));
		
		int testCase = 1;
		while(reader.ready())
		{
			int numLines = Integer.parseInt(reader.readLine());
			if(numLines == 0)
				break;
			
			Letter[] letters = null;
			for(int i = 0; i < numLines; i++)
			{
				String line = reader.readLine();
				String[] segments = line.split(" ");
				
				if(i == 0)
				{
					letters = new Letter[segments.length];
					for(int j = 0; j < segments.length; j++)
						letters[j] = new Letter(segments[j].length(), numLines);
				}
				
				for(int j = 0; j < letters.length; j++)
					letters[j].setLine(i, segments[j]);
			}
			
			Letter merged = process(letters);
			System.out.println(testCase++);
			System.out.print(merged);
		}

	}

	private static Letter process(Letter[] letters) 
	{
		Letter mergeLetter = letters[0];
		for(int i = 1; i < letters.length; i++)
			mergeLetter = mergeLetters(mergeLetter, letters[i]);

		mergeLetter.removeSeparators();
		
		Letter trimLetter = trimLetter(mergeLetter);
		
		return trimLetter;
	}
	
	private static Letter trimLetter(Letter letter) 
	{
		int start;
		boolean stop = false;
		for(start = 0; start < letter.width; start++)
		{
			for(int h = 0; h < letter.height; h++)
				if(!letter.isEmpty(start, h))
				{
					stop = true;
					break;
				}
			if(stop)
				break;
		}
		
		int end;
		stop = false;
		for(end = letter.width-1; end >= 0; end--)
		{
			for(int h = 0; h < letter.height; h++)
				if(!letter.isEmpty(end, h))
				{
					stop = true;
					break;
				}
			if(stop)
				break;
		}
		
		Letter trimLetter = new Letter(end - start + 1 , letter.height);
		for(int w = 0; w < trimLetter.width; w++)
			for(int h = 0; h < trimLetter.height; h++)
				trimLetter.setChar(w, h, letter.getChar(start + w, h));
		
		return trimLetter;
	}

	private static Letter mergeLetters(Letter letter1, Letter letter2)
	{
		int distance = calcMergeDistance(letter1, letter2);
		
		// calculate new length;
		int max = Math.max(letter1.width, letter2.width);
		int min = Math.min(letter1.width, letter2.width);
		int newWidth = max;
		if(distance < -max)
			newWidth = -distance;
		if(distance > -min)
			newWidth = max + min + distance;
		
		
		int offset = letter1.width + distance;
		
		Letter mergedLetter = new Letter(newWidth, letter1.height);
		for(int h = 0; h < mergedLetter.height; h++)
		{
			for(int w = 0; w < mergedLetter.width; w++)
			{
				mergedLetter.setChar(w,h,'.');

				int w1 = offset >= 0 ? w : w + offset;
				int w2 = offset >= 0 ? w - offset : w;
				
				
				if(w1 >= 0 && w1 < letter1.width)
				{
					if(!letter1.isEmpty(w1, h))
						mergedLetter.setChar(w, h, letter1.getChar(w1, h));
				}
				
				if(w2 >= 0 && w2 < letter2.width)
				{
					if(!letter2.isEmpty(w2, h))
					{
						if(!mergedLetter.isEmpty(w, h))
							throw new IllegalStateException("Merge failure");
						
						mergedLetter.setChar(w, h, letter2.getChar(w2, h));
					}
						
				}
			}
		}
		
		return mergedLetter;
	}
	
	/**
	 * merges letter1 and letter2 according to the merge rules
	 * we calculate the desired distance and merge the letters afterwards
	 */
	private static int calcMergeDistance(Letter letter1, Letter letter2)
	{
		// get position of the last char in every row of letter 1
		int letter1LastChar[] = new int[letter1.height];
		for(int h = 0; h < letter1.height; h++)
		{
			int max = -1000;
			for(int w = letter1.width-1; w >= 0; w--)
			{
				if(letter1.isEmpty(w, h))
					continue;
				
				max = w;
				break;
			}
			letter1LastChar[h] = max;
		}
		
		// get position of the first char in every row of letter 2
		int letter2FirstChar[] = new int[letter2.height];
		for(int h = 0; h < letter2.height; h++)
		{
			int min = 1000;
			for(int w = 0; w < letter2.width; w++)
			{
				if(letter2.isEmpty(w, h))
					continue;
				min = w;
				break;
			}
			letter2FirstChar[h] = min;
		}
		
		// calculate the minimum difference
		int minDiff = 1000;
		for(int h = 0; h < letter1.height; h++)
		{
			int diff = letter1.width + letter2FirstChar[h] - letter1LastChar[h];
			if(diff < minDiff)
				minDiff = diff;
		}
		
		return 2-minDiff;
	}
}

class Letter
{
	// width of the letter
	public final int width;
	// height of the letter
	public final int height;
	// the content of the letter, can be either '#', '.' or '0'
	private final char[][] data;

	public Letter(int width, int height)
	{
		this.width = width;
		this.height = height;
		data = new char[height][width];
	}
	
	public void removeSeparators() 
	{
		for(int h = 0; h < height; h++)
			for(int w = 0; w < width; w++)
			{
				if(data[h][w] == '0')
					data[h][w] = '.';
			}
	}

	public void setChar(int w, int h, char c) 
	{
		data[h][w] = c;
	}

	public char getChar(int w, int h)
	{
		return data[h][w];
	}
	
	public boolean isEmpty(int w, int h)
	{
		char c = getChar(w, h);
		return c != '#' && c != '0';
	}

	public void setLine(int line, String content) 
	{
		for(int w = 0; w < width; w++)
			data[line][w] = content.charAt(w);
	}
	
	@Override
	public String toString()
	{
		StringBuffer buffer = new StringBuffer();
		for(int h = 0; h < height; h++)
		{
			for(int w = 0; w < width; w++)
				buffer.append(data[h][w]);
			buffer.append('\n');
		}
		return buffer.toString();
	}
}
