import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.LinkedList;


public class Scavhunt 
{
	private static class StringSequence
	{
		private LinkedList<String> sequence = new LinkedList<String>();

		public StringSequence(String begin, String end) 
		{
			sequence.add(begin);
			sequence.add(end);
		}
		
		public void append(StringSequence other)
		{
			if(!this.sequence.getLast().equals(other.sequence.getFirst()))
				throw new IllegalStateException("Sequences cannot be joined!");
			this.sequence.removeLast();
			this.sequence.addAll(other.sequence);
		}
		
		@Override
		public String toString() 
		{
			String out = "";
			for(String s : sequence)
				out += s + "\n";
			return out;
		}

		public String getLast() 
		{
			return sequence.getLast();
		}
	}
	
	public static void main(String[] args) throws IOException 
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		//BufferedReader reader = new BufferedReader(new FileReader("scavenger.txt"));

		int numTestCases = Integer.parseInt(reader.readLine());
		for(int scenarioIndex = 1; scenarioIndex <= numTestCases; scenarioIndex++)
		{
			int numElements = Integer.parseInt(reader.readLine());
			HashMap<String, StringSequence> beginSequenceMap = new HashMap<String, StringSequence>();
			HashMap<String, StringSequence> endSequenceMap = new HashMap<String, StringSequence>();
			while(--numElements>0)
			{
				String[] pair = reader.readLine().split(" ");
				StringSequence sequence = new StringSequence(pair[0], pair[1]);
				beginSequenceMap.put(pair[0], sequence);
				endSequenceMap.put(pair[1], sequence);
			}
			
			while(beginSequenceMap.size() > 1 && endSequenceMap.size() > 0)
			{
				String endKey = endSequenceMap.keySet().iterator().next();
				StringSequence beginSeq = endSequenceMap.remove(endKey);
				StringSequence endSeq = beginSequenceMap.get(endKey);
				if(endSeq == null)
					continue;
				
				beginSeq.append(endSeq);
				beginSequenceMap.remove(endKey);
				endSequenceMap.put(beginSeq.getLast(), beginSeq);
			}
			StringSequence concSequence = beginSequenceMap.get(beginSequenceMap.keySet().iterator().next());
			System.out.println("Scenario #" + scenarioIndex + ":");
			System.out.println(concSequence);
			
		}

	}

}
