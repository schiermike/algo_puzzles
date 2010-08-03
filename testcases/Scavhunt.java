import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;


public class SCAVHUNT_TestCaseGenerator 
{
	private static class StringPair
	{
		private final int z1;
		private final int z2;

		public StringPair(int z1, int z2)
		{
			this.z1 = z1;
			this.z2 = z2;
		}
		
		@Override
		public String toString() 
		{
			return z1 + " " + z2;
		}
	}
	
	public static void main(String[] args) throws IOException 
	{
		FileWriter writer = new FileWriter("scavenger.txt");
		
		ArrayList<StringPair> pairs = new ArrayList<StringPair>();
		int size = 1000;
		
		for(int i = 0; i < size-1; i++)
			pairs.add(new StringPair(i, i+1));
		
		Collections.shuffle(pairs);
		writer.write("1\n");
		writer.write(size + "\n");
		for(StringPair pair : pairs)
			writer.write(pair + "\n");
		writer.close();
	}

}
