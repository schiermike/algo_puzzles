import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.HashSet;
import java.util.Random;


public class MATCHING_TestCaseGenerator 
{
	public static void main(String[] args) throws IOException 
	{
		String name = "testcases/MATCHING_s0.txt";
		randMatching(new FileWriter(name));
		
		long time = System.currentTimeMillis();
		Matching.process(new FileReader(name));
		time = System.currentTimeMillis() - time;
		System.out.println(time + "ms");

	}

	private static void randMatching(Writer w) throws IOException 
	{
		BufferedWriter writer = new BufferedWriter(w);
		
		int n = 6;
		int m = 10000;
		int p = 0;
		
		Random r = new Random();
		StringBuffer buffer = new StringBuffer();
		HashSet<Integer> used = new HashSet<Integer>();
		for(int x = 1; x <= n; x++)
		{
			while(r.nextDouble() < 0.7)
			{
				int y = r.nextInt(m)+1;
				if(used.add(y))
				{
					buffer.append(x + " " + y + "\n");
					p++;
				}
			}
			used.clear();
		}
		
		System.out.println(p + " edges generated.");
		writer.write(n + " " + m + " " + p + "\n");
		writer.write(buffer.toString());
		writer.close();
	}

}
