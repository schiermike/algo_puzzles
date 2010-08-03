import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;

public class Bulk 
{
	private static final HashSet<Face> faces = new HashSet<Face>();
	private static final HashSet<Vertex> blocks = new HashSet<Vertex>();
	private static final HashMap<Orientation, HashSet<Vertex>> borderSquares = new HashMap<Orientation, HashSet<Vertex>>();
	
	public static void main(String[] args) throws IOException 
	{
		borderSquares.put(Orientation.xyFace, new HashSet<Vertex>());
		borderSquares.put(Orientation.xzFace, new HashSet<Vertex>());
		borderSquares.put(Orientation.yzFace, new HashSet<Vertex>());
		
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		if(!reader.ready())
			reader = new BufferedReader(new FileReader("testcases/BULK.txt"));
		
		int numTests = Integer.parseInt(reader.readLine());
		while(numTests-->0)
		{
			faces.clear();
			int numFaces = Integer.parseInt(reader.readLine());
			while(numFaces-->0)
			{
				String line = reader.readLine();
				int start = line.indexOf(' ') + 2;
				int end = 0;
				Face face = new Face();
				while(end >= 0)
				{
					end = line.indexOf(' ', start);
					int x = Integer.parseInt(line.substring(start, end));
					start = end + 1;
					end = line.indexOf(' ', start);
					int y = Integer.parseInt(line.substring(start, end));
					start = end + 1;
					end = line.indexOf(' ', start);
					int z = end >= 0 ? Integer.parseInt(line.substring(start, end)) : Integer.parseInt(line.substring(start));
					start = end + 2;
					face.addCornerVertex(x, y, z);
				}
				faces.add(face);
			}
			solve();
		}

	}

	private static void solve() 
	{
		borderSquares.get(Orientation.xyFace).clear();
		borderSquares.get(Orientation.xzFace).clear();
		borderSquares.get(Orientation.yzFace).clear();
		
		for(Face face : faces)
		{
			// derive all Squares of this face
			face.deriveSquares();
			
			// put all squares of faces with the same orientation together
			borderSquares.get(face.getOrientation()).addAll(face.getSquares());
		}
		
		Vertex min = null;
		for(Vertex v : borderSquares.get(Orientation.xyFace))
		{
			if(min == null || v.compareTo(min) < 0)
				min = v;
		}
		
		blocks.clear();
		blocks.add(min);
		collectBlocks(min);
		System.out.println("The bulk is composed of " + blocks.size() + " units.");
		
	}

	private static void collectBlocks(Vertex start) 
	{
		LinkedList<Vertex> queue = new LinkedList<Vertex>();
		queue.add(start);
		
		while(!queue.isEmpty())
		{
			Vertex v = queue.removeFirst();
			tryGoLeft(queue, v);
			tryGoRight(queue, v);
			tryGoDown(queue, v);
			tryGoUp(queue, v);
			tryGoBack(queue, v);
			tryGoFront(queue, v);
		}
	}
	
	private static void tryGoLeft(LinkedList<Vertex> queue, Vertex v) 
	{
		if(borderSquares.get(Orientation.yzFace).contains(v))
			return;

		Vertex next = new Vertex(v.x-1, v.y, v.z);
		if(blocks.contains(next))
			return;
		
		blocks.add(next);
		queue.add(next);
	}

	private static void tryGoRight(LinkedList<Vertex> queue, Vertex v) 
	{
		Vertex next = new Vertex(v.x+1, v.y, v.z);
		if(borderSquares.get(Orientation.yzFace).contains(next))
			return;
		
		if(blocks.contains(next))
			return;
		
		blocks.add(next);
		queue.add(next);
	}
	
	private static void tryGoDown(LinkedList<Vertex> queue, Vertex v) 
	{
		if(borderSquares.get(Orientation.xzFace).contains(v))
			return;

		Vertex next = new Vertex(v.x, v.y-1, v.z);
		if(blocks.contains(next))
			return;
		
		blocks.add(next);
		queue.add(next);
	}
	
	private static void tryGoUp(LinkedList<Vertex> queue, Vertex v) 
	{
		Vertex next = new Vertex(v.x, v.y+1, v.z);
		if(borderSquares.get(Orientation.xzFace).contains(next))
			return;
		
		if(blocks.contains(next))
			return;
		
		blocks.add(next);
		queue.add(next);
	}
	
	private static void tryGoBack(LinkedList<Vertex> queue, Vertex v) 
	{
		if(borderSquares.get(Orientation.xyFace).contains(v))
			return;

		Vertex next = new Vertex(v.x, v.y, v.z-1);
		if(blocks.contains(next))
			return;
		
		blocks.add(next);
		queue.add(next);
	}
	
	private static void tryGoFront(LinkedList<Vertex> queue, Vertex v) 
	{
		Vertex next = new Vertex(v.x, v.y, v.z+1);
		if(borderSquares.get(Orientation.xyFace).contains(next))
			return;
		
		if(blocks.contains(next))
			return;
		
		blocks.add(next);
		queue.add(next);
	}
}

enum Orientation
{
	xyFace,
	xzFace,
	yzFace
}

class Face
{
	private final LinkedList<Vertex> cornerVertices = new LinkedList<Vertex>();
	private final LinkedList<Vertex> borderVertices = new LinkedList<Vertex>();
	private final HashSet<Vertex> squares = new HashSet<Vertex>();
	
	public HashSet<Vertex> getSquares() 
	{
		return squares;
	}

	private Orientation orientation = null;
	
	public Orientation getOrientation()
	{
		if(orientation == null)
		{
			boolean xChanges = false;
			boolean yChanges = false;
			boolean zChanges = false;
			Vertex p = cornerVertices.getFirst();
			for(Vertex p2 : cornerVertices)
			{
				xChanges |= p.x != p2.x;
				yChanges |= p.y != p2.y;
				zChanges |= p.z != p2.z;
			}
			
			if(xChanges && yChanges)
				orientation = Orientation.xyFace;
			if(xChanges && zChanges)
				orientation = Orientation.xzFace;
			if(yChanges && zChanges)
				orientation = Orientation.yzFace;
		}
		return orientation;
	}

	public void addCornerVertex(int x, int y, int z) 
	{
		cornerVertices.add(new Vertex(x,y,z));
	}
	
	public void deriveSquares()
	{
		deriveBorderVertices();
		squares.clear();
		
		Vertex min = null;
		for(Vertex v : cornerVertices)
			if(min == null || v.compareTo(min) < 0)
				min = v;

		// min = starting point
		squares.add(min);
		collectSquares(min);
	}
	
	private void collectSquares(Vertex start) 
	{
		// try all 4 directions
		LinkedList<Vertex> queue = new LinkedList<Vertex>();
		queue.add(start);
		
		while(!queue.isEmpty())
		{
			Vertex v = queue.removeFirst();
			tryGoLeft(queue, v);
			tryGoRight(queue, v);
			tryGoUp(queue, v);		
			tryGoDown(queue, v);
		}
	}
	
	private void tryGoLeft(LinkedList<Vertex> queue, Vertex v) 
	{
		Vertex next = null;
		switch(getOrientation())
		{
			case xyFace:
				if(borderContainsEdge(new Vertex(v.x, v.y, v.z), new Vertex(v.x, v.y+1, v.z)))
					return;
				next = new Vertex(v.x-1, v.y, v.z);
				break;
			case xzFace:
				if(borderContainsEdge(new Vertex(v.x, v.y, v.z), new Vertex(v.x, v.y, v.z+1)))
					return;
				next = new Vertex(v.x-1, v.y, v.z);
				break;
			case yzFace:
				if(borderContainsEdge(new Vertex(v.x, v.y, v.z), new Vertex(v.x, v.y, v.z+1)))
					return;
				next = new Vertex(v.x, v.y-1, v.z);
				break;
		}
		if(squares.contains(next))
			return;
		
		squares.add(next);
		queue.addLast(next);
	}
	
	private void tryGoRight(LinkedList<Vertex> queue, Vertex v) 
	{
		Vertex next = null;
		switch(getOrientation())
		{
			case xyFace:
				if(borderContainsEdge(new Vertex(v.x+1, v.y, v.z), new Vertex(v.x+1, v.y+1, v.z)))
					return;
				next = new Vertex(v.x+1, v.y, v.z);
				break;
			case xzFace:
				if(borderContainsEdge(new Vertex(v.x+1, v.y, v.z), new Vertex(v.x+1, v.y, v.z+1)))
					return;
				next = new Vertex(v.x+1, v.y, v.z);
				break;
			case yzFace:
				if(borderContainsEdge(new Vertex(v.x, v.y+1, v.z), new Vertex(v.x, v.y+1, v.z+1)))
					return;
				next = new Vertex(v.x, v.y+1, v.z);
				break;
		}
		if(squares.contains(next))
			return;
		
		squares.add(next);
		queue.addLast(next);
	}

	private void tryGoUp(LinkedList<Vertex> queue, Vertex v) 
	{
		Vertex next = null;
		switch(getOrientation())
		{
			case xyFace:
				if(borderContainsEdge(new Vertex(v.x, v.y+1, v.z), new Vertex(v.x+1, v.y+1, v.z)))
					return;
				next = new Vertex(v.x, v.y+1, v.z);
				break;
			case xzFace:
				if(borderContainsEdge(new Vertex(v.x, v.y, v.z+1), new Vertex(v.x+1, v.y, v.z+1)))
					return;
				next = new Vertex(v.x, v.y, v.z+1);
				break;
			case yzFace:
				if(borderContainsEdge(new Vertex(v.x, v.y, v.z+1), new Vertex(v.x, v.y+1, v.z+1)))
					return;
				next = new Vertex(v.x, v.y, v.z+1);
				break;
		}
		if(squares.contains(next))
			return;
		
		squares.add(next);
		queue.addLast(next);
	}
	
	private void tryGoDown(LinkedList<Vertex> queue, Vertex v) 
	{
		Vertex next = null;
		switch(getOrientation())
		{
			case xyFace:
				if(borderContainsEdge(new Vertex(v.x, v.y, v.z), new Vertex(v.x+1, v.y, v.z)))
					return;
				next = new Vertex(v.x, v.y-1, v.z);
				break;
			case xzFace:
				if(borderContainsEdge(new Vertex(v.x, v.y, v.z), new Vertex(v.x+1, v.y, v.z)))
					return;
				next = new Vertex(v.x, v.y, v.z-1);
				break;
			case yzFace:
				if(borderContainsEdge(new Vertex(v.x, v.y, v.z), new Vertex(v.x, v.y+1, v.z)))
					return;
				next = new Vertex(v.x, v.y, v.z-1);
				break;
		}
		if(squares.contains(next))
			return;
		
		squares.add(next);
		queue.addLast(next);
	}
	

	private boolean borderContainsEdge(Vertex vertex1, Vertex vertex2) 
	{
		int i = borderVertices.indexOf(vertex1);
		if(i == -1)
			return false;
		if(borderVertices.get((i+1) % borderVertices.size()).equals(vertex2))
			return true;
		if(borderVertices.get((i+borderVertices.size()-1) % borderVertices.size()).equals(vertex2))
			return true;

		return false;
	}

	private void deriveBorderVertices()
	{
		borderVertices.clear();
		Iterator<Vertex> cornerIterator = cornerVertices.iterator();
		Vertex last = null;
		Vertex next = cornerIterator.next();
		while(cornerIterator.hasNext())
		{
			last = next;
			next = cornerIterator.next();
			addPointsBetween(last, next);
		}
		last = next;
		next = cornerVertices.getFirst();
		addPointsBetween(last, next);
		
	}

	private void addPointsBetween(Vertex v1, Vertex v2) 
	{
		if(v1.y == v2.y && v1.z == v2.z) // x changes
		{
			
			for(int i = v1.x; i != v2.x; i=(v1.x < v2.x ? i+1 : i-1))
				borderVertices.add(new Vertex(i, v1.y, v1.z));
		}
		else if(v1.x == v2.x && v1.z == v2.z) // y changes
		{
			for(int i = v1.y; i != v2.y; i=(v1.y < v2.y ? i+1 : i-1))
				borderVertices.add(new Vertex(v1.x, i, v1.z));
		}
		else if(v1.x == v2.x && v1.y == v2.y) // z changes 
		{
			for(int i = v1.z; i != v2.z; i=(v1.z < v2.z ? i+1 : i-1))
				borderVertices.add(new Vertex(v1.x, v1.y, i));
		}
	}
}

class Vertex implements Comparable<Vertex>
{
	public int x;
	public int y;
	public int z;

	public Vertex(int x, int y, int z) 
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public Vertex(Vertex other) 
	{
		this.x = other.x;
		this.y = other.y;
		this.z = other.z;
	}

	@Override
	public int compareTo(Vertex o) 
	{
		if(this.x - o.x != 0)
			return this.x - o.x;
		if(this.y - o.y != 0)
			return this.y - o.y;
		return this.z - o.z;
	}
	
	@Override
	public int hashCode() 
	{
		return (this.x << 20) + (this.y << 10) + this.z;
	}
	
	@Override
	public boolean equals(Object other) 
	{
		if(!(other instanceof Vertex))
			return false;
		Vertex vertex2 = (Vertex)other;

		return this.x == vertex2.x && this.y == vertex2.y && this.z == vertex2.z;
	}
	
	@Override
	public String toString() 
	{
		return "(" + x + "," + y + "," + z + ")";
	}
}
