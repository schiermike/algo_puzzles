import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;


public class Test 
{
	public static void main(String[] args) throws IOException 
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		
		while(reader.ready())
		{
			String line = reader.readLine();
			if(line.equals("42"))
				break;
			System.out.println(line);
		}

	}

}
