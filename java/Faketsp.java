import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.DecimalFormat;


public class Faketsp 
{
	public static void main(String[] args) throws IOException 
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		//BufferedReader reader = new BufferedReader(new FileReader("testcases/FAKETSP.txt"));
		
		double distance = 0;
		double xOld = 0;
		double yOld = 0;
		boolean started = false;
		DecimalFormat format = new DecimalFormat("0.000");
		while(reader.ready())
		{
			double[] coords = parseCoords(reader.readLine());
			if(!started)
			{
				xOld = coords[0];
				yOld = coords[1];
				started = true;
				continue;
			}
			
			double temp = (coords[0]-xOld)*(coords[0]-xOld) + (coords[1]-yOld)*(coords[1]-yOld);
			distance += Math.sqrt(temp);
			xOld = coords[0];
			yOld = coords[1];
			
			System.out.println("The salesman has traveled a total of " + format.format(distance) + " kilometers.");
		}
	}

	private static double[] parseCoords(String line) 
	{
		int x = line.indexOf('(');
		int y = line.indexOf(')');
		line = line.substring(x+1, y);
		String[] s = line.split(",");
		return new double[]{ Double.parseDouble(s[0]), Double.parseDouble(s[1]) };
	}
}
