import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.DecimalFormat;
import java.util.HashMap;


public class Knightsr
{
	private static HashMap<Integer, Double> lookupTable_f = new HashMap<Integer, Double>();
	private static HashMap<Integer, Double> lookupTable_sumf = new HashMap<Integer, Double>();
	private static int maxVal = 2;
	
	public static void main(String[] args) throws NumberFormatException, IOException
	{
		lookupTable_f.put(1, 0.0);
		lookupTable_sumf.put(1,0.0);
		lookupTable_f.put(2, 0.5);
		lookupTable_sumf.put(2,0.5);
		
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		while(reader.ready())
		{
			int input = Integer.parseInt(reader.readLine());
			if(input == 0)
				break;
			
			double output = f(input);
			DecimalFormat format = new DecimalFormat("#.##");
			System.out.println("With " + input + " competitors, a Jedi Knight will be replaced approximately " + format.format(output) + " times.\n");
		}
	}
	
	private static double f(int i)
	{
		if(i>maxVal)
		{
			for(int val = maxVal + 1; val <= i; val++)
				calcF(val);
		}
		
		return lookupTable_f.get(i);
	}

	private static void calcF(int val)
	{
		double sumNmin1 = lookupTable_sumf.get(val-1);
		double d = (val - 1 + sumNmin1)/val;
		lookupTable_f.put(val, d);
		lookupTable_sumf.put(val, sumNmin1 + d);
	}

}
