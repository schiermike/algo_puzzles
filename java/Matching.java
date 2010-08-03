import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;

public class Matching 
{
	private static HashMap<Integer, HashSet<Edge>> vertexToEdge;
	
	private static boolean[] edgesInPairing;
	private static boolean[] vertexInPairing;
	private static HashSet<Integer> vertexANotInPairing;
	
	private static int numVerticesA;
	private static int numVerticesB;
	private static int numEdges;
	
	public static void main(String[] args) throws IOException
	{
		Reader reader = new InputStreamReader(System.in);
		if(!reader.ready())
			reader = new FileReader(args.length > 0 ? args[0] : "testcases/MATCHING_s3.txt");
		
		System.out.println(process(reader));
	}
	
	public static int process(Reader r) throws IOException
	{
		BufferedReader reader = new BufferedReader(r);
		
		String line = reader.readLine();
		String[] firstLine = line.split(" ");

		numVerticesA = Integer.parseInt(firstLine[0]);
		numVerticesB = Integer.parseInt(firstLine[1]);
		numEdges = Integer.parseInt(firstLine[2]);
		
		vertexToEdge = new HashMap<Integer, HashSet<Edge>>();
		edgesInPairing = new boolean[numEdges];
		vertexInPairing = new boolean[numVerticesA+numVerticesB];
		vertexANotInPairing = new HashSet<Integer>();
		
		int edgeID = 0;
		int tmp;
		while(reader.ready())
		{
			line = reader.readLine();
			
			tmp = line.indexOf(' ');
			int a = Integer.parseInt(line.substring(0, tmp));
			int b = Integer.parseInt(line.substring(tmp+1));
			Edge edge = new Edge(edgeID++, numVerticesA - a, numVerticesA + numVerticesB - b);
			
			// build x->Pair lookup table
			HashSet<Edge> aEdges = vertexToEdge.get(edge.vertexA);
			if(aEdges == null)
			{
				aEdges = new HashSet<Edge>();
				vertexToEdge.put(edge.vertexA, aEdges);
			}
			aEdges.add(edge);
			
			// build y->Pair lookup table
			HashSet<Edge> bEdges = vertexToEdge.get(edge.vertexB);
			if(bEdges == null)
			{
				bEdges = new HashSet<Edge>();
				vertexToEdge.put(edge.vertexB, bEdges);
			}
			bEdges.add(edge);
			
			// create initial pairing
			if(!vertexInPairing[edge.vertexA] && !vertexInPairing[edge.vertexB])
			{
				edgesInPairing[edge.id] = true;
				vertexInPairing[edge.vertexA] = true;
				vertexInPairing[edge.vertexB] = true;
			}
		}
		
		// create set of free nodes
		for(int vertex = 0; vertex < numVerticesA; vertex++)
			if(!vertexInPairing[vertex])
				vertexANotInPairing.add(vertex);
		
		while(breathSearch()){}
		
		return numVerticesA - vertexANotInPairing.size();
	}
	
	private static boolean breathSearch()
	{
		boolean success = false;
		Iterator<Integer> startVertexIterator = vertexANotInPairing.iterator();
		LinkedList<Path> prolongablePaths = new LinkedList<Path>();
		
		boolean[] lockedVertices = new boolean[numVerticesA + numVerticesB];
		
		int firstVertex;
		int lastVertex;
		Path path;
		while(true)
		{
			// first start with path length 0 and use all vertices from vertexANotInPairing as starting point
			if(startVertexIterator.hasNext())
			{
				firstVertex = startVertexIterator.next();
				lastVertex = firstVertex;
				path = new Path();
			}
			else if(!prolongablePaths.isEmpty())// then continue with prologing existing paths
			{
				path = prolongablePaths.removeFirst();
				firstVertex = path.getFirstVertex();
				lastVertex = path.getLastVertex();
			}
			else
				break;
				
			if(lockedVertices[firstVertex])
				continue;
			
			if(vertexInPairing[firstVertex])
			{
				for(int v : path.getVertices())
					lockedVertices[v] = false;
				continue;
			}
			
			if(!vertexToEdge.containsKey(lastVertex))
				continue;
			
			for(Edge edge : vertexToEdge.get(lastVertex))
			{
				boolean edgeGoesFromAToB = (path.size() % 2 == 0);
				
				int vertexCandidate = edgeGoesFromAToB ? edge.vertexB : edge.vertexA;
				
				// we already visited this node earlier and found not path
				if(lockedVertices[vertexCandidate])
					continue;
				
				// we have to use an alternating path, uneven path segments have to belong to the edgePairing
				if(edgesInPairing[edge.id] == edgeGoesFromAToB)
					continue;
				
				// we append the edge to the path
				Path prolongedPath = new Path(path);
				prolongedPath.add(edge);
				lockedVertices[vertexCandidate] = true;
				
				// we found a path!!
				if(!vertexInPairing[vertexCandidate])
				{
					// invert the path
					invertPath(prolongedPath);
					for(int v : prolongedPath.getVertices())
						lockedVertices[v] = false;
					success = true;
					
					// we have to continue our search
					break;
				}
				
				// only try to look for longer paths when we haven't found any paths of this length
				prolongablePaths.addLast(prolongedPath);
			}
			
		}
		return success;
	}

	private static void invertPath(Path path) 
	{
		// path starts in B and ends in set A
		if(path.size()>=2 && path.getFirst().vertexA == path.get(1).vertexA)
		{
			vertexInPairing[path.getFirst().vertexB] = true;
			vertexInPairing[path.getLast().vertexA] = true;
			vertexANotInPairing.remove(path.getLast().vertexA);
		}
		else // path starts in A and ends in set B
		{
			vertexInPairing[path.getFirst().vertexA] = true;
			vertexInPairing[path.getLast().vertexB] = true;
			vertexANotInPairing.remove(path.getFirst().vertexA);
		}
		for(int i = 0; i < path.size(); i++)
		{
			if(i%2 == 0)
			{
				edgesInPairing[path.get(i).id] = true;
			}
			else
			{
				edgesInPairing[path.get(i).id] = false;
			}
		}
	}
}

class Edge
{
	public final int vertexA;
	public final int vertexB;
	public final int id;
	
	public Edge(int id, int vertexA, int vertexB) 
	{
		this.id = id;
		this.vertexA = vertexA;
		this.vertexB = vertexB;
	}
	
	@Override
	public int hashCode()
	{
		return id;
	}

	@Override
	public String toString()
	{
		return "{" + vertexA + "," + vertexB + "}";
	}
}

@SuppressWarnings("serial")
class Path extends LinkedList<Edge> 
{
	public Path()
	{
		super();
	}
	
	public Path(Path other)
	{
		super(other);
	}
	
	public HashSet<Integer> getVertices() 
	{
		HashSet<Integer> vertices = new HashSet<Integer>();
		for(Edge edge : this)
		{
			vertices.add(edge.vertexA);
			vertices.add(edge.vertexB);
		}
		return vertices;
	}

	public int getLastVertex() 
	{
		if(this.size() == 1)
			return getFirst().vertexB;
		else
			return this.size() % 2 == 0 ? getLast().vertexA : getLast().vertexB;
	}

	public int getFirstVertex() 
	{
		return getFirst().vertexA;
	}
}

@SuppressWarnings("serial")
class TerminationException extends RuntimeException {}