import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;


public class Javac 
{
	public static void main(String[] args) throws IOException 
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		if(!reader.ready())
			reader = new BufferedReader(new FileReader("testcases/JAVAC.txt"));
		
		while(reader.ready())
			System.out.println(transformIdentifier(reader.readLine()));
	}

	private static String transformIdentifier(String identifier) 
	{
		boolean containsUnderscore = identifier.contains("_");
		boolean containsUppercase = !identifier.toLowerCase().equals(identifier);
		
		// same style in JAVA and C++
		if(!containsUnderscore && !containsUppercase)
			return identifier;
		// identifier does not follow any language formatting rule
		if(containsUnderscore && containsUppercase 
			|| identifier.startsWith("_") || identifier.endsWith("_") || identifier.contains("__")
			|| Character.isUpperCase(identifier.charAt(0)))
			return "Error!";
		
		if(containsUnderscore)
			return convertToJavaStyle(identifier);
		
		return convertToCppStyle(identifier);
	}

	private static String convertToCppStyle(String identifier) 
	{
		StringBuffer converted = new StringBuffer();
		
		for(char c : identifier.toCharArray())
		{
			if(Character.isUpperCase(c))
				converted.append("_" + Character.toLowerCase(c));
			else
				converted.append(c);
		}
		
		return converted.toString();
	}

	private static String convertToJavaStyle(String identifier) 
	{
		StringBuffer converted = new StringBuffer();
		
		boolean preceededByUnderscore = false;
		for(char c : identifier.toCharArray())
		{
			if(c == '_')
				preceededByUnderscore = true;
			else
			{
				if(preceededByUnderscore)
				{
					converted.append(Character.toUpperCase(c));
					preceededByUnderscore = false;
				}
				else
					converted.append(c);
			}
		}
		
		return converted.toString();
	}

}
