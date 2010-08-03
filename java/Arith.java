import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.math.BigInteger;


public class Arith 
{
	public static void main(String[] args) throws IOException 
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		if(!reader.ready())
			reader = new BufferedReader(new FileReader("testcases/ARITH.txt"));

		int numTests = Integer.parseInt(reader.readLine());
		while(numTests-->0)
		{
			String expression = reader.readLine();
			int separation = expression.indexOf('+');
			if(separation == -1)
				separation = expression.indexOf('-');
			if(separation == -1)
				separation = expression.indexOf('*');
			BigInteger num1 = new BigInteger(expression.substring(0, separation));
			BigInteger num2 = new BigInteger(expression.substring(separation+1));
			switch(expression.charAt(separation))
			{
				case '+':
					add(num1, num2);
					break;
				case '-':
					sub(num1, num2);
					break;
				case '*':
					mult(num1, num2);
					break;
			}
			System.out.println(""); // separator line
		}

		
	}
	
	private static String spaces = "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    ";
	private static String separators = "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
	
	private static void add(BigInteger num1, BigInteger num2) 
	{
		String s1 = num1.toString();
		String s2 = num2.toString();
		String result = num1.add(num2).toString();
		
		int max = s1.length() > s2.length() ? s1.length() : s2.length() + 1;
		max = result.length() > max ? result.length() : max;
		
		print(max - s1.length(), s1);
		print(max - s2.length() - 1, "+" + s2);
		
		int max2 = result.length() > s2.length() ? result.length() : s2.length()+1;
		print(max-max2, separators.substring(0, max2));

		print(max - result.length(), result);
	}

	private static void sub(BigInteger num1, BigInteger num2) 
	{
		String s1 = num1.toString();
		String s2 = num2.toString();
		String result = num1.subtract(num2).toString();
		
		int max = s1.length() > s2.length() ? s1.length() : s2.length() + 1;
		max = result.length() > max ? result.length() : max;
		
		print(max - s1.length(), s1);
		print(max - s2.length() - 1, "-" + s2);
		
		int max2 = result.length() > s2.length() ? result.length() : s2.length()+1;
		print(max-max2, separators.substring(0, max2));

		print(max - result.length(), result);
	}

	private static void mult(BigInteger num1, BigInteger num2) 
	{
		String s1 = num1.toString();
		String s2 = num2.toString();
		String result = num1.multiply(num2).toString();
		
		int max = s1.length() > s2.length() ? s1.length() : s2.length() + 1;
		max = result.length() >max ? result.length() : max;
		
		print(max - s1.length(), s1);
		print(max - s2.length() - 1, "*" + s2);
		
		if(s2.length() > 1)
		{
			int tmp1Len = num1.multiply(new BigInteger("" + s2.charAt(s2.length()-1))).toString().length();
			
			int max2 = tmp1Len > s2.length() ? tmp1Len : s2.length() + 1;
			print(max - max2, separators.substring(max - max2, max));
			
			for(int i = 0; i < s2.length(); i++)
			{
				String tmp = num1.multiply(new BigInteger("" + s2.charAt(s2.length()-i-1))).toString();
				print(max - tmp.length() - i, tmp);
			}
		}
		
		int max2 = result.length() > 2 ? result.length() : 2;
		print(max - max2, separators.substring(0, max2));
		print(max - result.length(), result);
		
	}
	
	private static void print(int offset, String string)
	{
		if(offset > 0)
			System.out.print(spaces.substring(0, offset));
		System.out.println(string);
	}
	
}
