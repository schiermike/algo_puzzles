import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.LinkedList;
import java.util.Random;


/**
10
999000000 999100000
999100000 999200000
999200000 999300000
999300000 999400000
999400000 999500000
999500000 999600000
999600000 999700000
999700000 999800000
999800000 999900000
999900000 1000000000
 */

public class Prime1 
{
	private static LinkedList<Integer> divisors;
	private static StringBuffer outputBuffer = new StringBuffer();

	public static void main(String[] args) throws FileNotFoundException, IOException 
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		if(!reader.ready())
			reader = new BufferedReader(new FileReader("testcases/PRIME1.txt"));
		
		int numTests = Integer.parseInt(reader.readLine());
		int input[][] = new int[numTests][2];
		
		int max = 0;
		for(int i = 0; i < numTests; i++)
		{
			String line = reader.readLine();
			int sepIndex = line.indexOf(' ');
			input[i][0] = Integer.parseInt(line.substring(0, sepIndex));
			input[i][1] = Integer.parseInt(line.substring(sepIndex+1));
			if(max < input[i][1])
				max = input[i][1];
		}
		
//		createDivisorCandidates(isqrt(max));
//		
//		for(int i = 0; i < numTests; i++)
//			findPrimes(input[i][0], input[i][1]);
		
		for(int i = 0; i < numTests; i++)
		{
			if(i!=0)
				System.out.println("\n");
			findPrimesMR(input[i][0], input[i][1]);
			System.out.print(outputBuffer.toString());
			outputBuffer = new StringBuffer();
		}
	}

	private static void findPrimesMR(int min, int max) 
	{
		for(int candidate = min; candidate <= max; candidate++)
			if(isPrimeMillerRabin(candidate))
				outputBuffer.append(candidate + "\n");
		outputBuffer.delete(outputBuffer.length()-1, outputBuffer.length());
	}

	private static Random random = new Random();
	private static boolean isPrimeMillerRabin(int n) 
	{
		if(
				n <= 1 || 
				n % 2 == 0 && n != 2 || 
				n % 3 == 0 && n != 3 || 
				n % 5 == 0 && n != 5 ||
				n % 7 == 0 && n != 7 ||
				n % 11 == 0 && n != 11 ||
				n % 13 == 0 && n != 13 ||
				n % 17 == 0 && n != 17 ||
				n % 19 == 0 && n != 19 ||
				n % 23 == 0 && n != 23 ||
				n % 29 == 0 && n != 29 ||
				n % 31 == 0 && n != 31 ||
				n % 37 == 0 && n != 37
		  )
			return false;
		
		int r = 10;
		int d = n;
		int s = 0;
		while(d % 2 == 0)
		{
			d >>= 1;
			s++;
			if(d == 0)
				return false;
		}
		
		while(r-->=0)
		{
			int a = 1 + random.nextInt(n-1);
			int tmp = pow(a, d);
			if(tmp % n == 1)
				continue;
			
			if(s>0)
			{
				boolean check = true;
				for(int t = 0; t <= s-1; t++)
				{
					if(tmp % n == n-1)
					{
						check = false;
						break;
					}
					tmp = tmp * tmp;
				}
				if(check)
					return false;
			}
		}
		
		return true;
	}
	
	// fast exponentiation
	private static int pow(int a, int b)
	{
		int result = 0;
		while(b > 0)
		{
			if(b % 2 == 1)
				result += a;
			b = b/2;
			a = a*a;
		}
		return result;
	}
	
	private static int next(int n, int i) 
	{
		return (n + i/n) >> 1;
	}

	public static int isqrt(int number) 
	{
		int n  = 1;
		int n1 = next(n, number);

		while(Math.abs(n1 - n) > 1) 
		{
			n  = n1;
			n1 = next(n, number);
		}
		while((n1*n1) > number) 
	      n1 -= 1;
		return n1;
	}


	@SuppressWarnings("unused")
	private static void createDivisorCandidates(int maxCandidate) 
	{
		divisors = new LinkedList<Integer>();
		divisors.add(2);
		divisors.add(5);
		divisors.add(7);
		divisors.add(11);
		
		boolean isPrime;
		int sqrtCandidate;
		for(int candidate = 13; candidate <= maxCandidate; candidate++)
		{
			sqrtCandidate = isqrt(candidate);
			isPrime = true;
			for(int prime : divisors)
			{
				if(prime > sqrtCandidate)
					break;
				if(candidate % prime == 0)
				{
					isPrime = false;
					break;
				}
			}
			if(isPrime)
				divisors.addLast(candidate);
				
		}
	}

	@SuppressWarnings("unused")
	private static void findPrimes(int min, int max) 
	{
		int sqrtCandidate;
		boolean isPrime;
		if(min % 2 == 0)
			min++;
		
		for(int candidate = min; candidate < max; candidate+=2)
		{
			if(candidate % 11 == 0 || candidate % 7 == 0 || candidate % 5 == 0 || candidate % 3 == 0)
				continue;
			
			sqrtCandidate = (int)Math.sqrt(candidate);
			isPrime = true;
			for(int prime : divisors)
			{
				if(prime > sqrtCandidate)
					break;
				if(candidate % prime == 0)
				{
					isPrime = false;
					break;
				}
			}
			if(isPrime)
				outputBuffer.append(candidate + "\n");
		}
		
		outputBuffer.append("\n");
	}

}
