#include <string>
#include <tr1/unordered_set>
#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <sys/time.h>

using namespace std;
using namespace tr1;

unsigned int getMicroSecs()
{
	struct timeval timeVal;
	gettimeofday(&timeVal, 0);
	return timeVal.tv_sec * 1000000 + timeVal.tv_usec;
}

enum Orientation
{
	xFace,
	yFace,
	zFace
};

enum Direction
{
	leftDir,
	rightDir,
	upDir,
	downDir,
	frontDir,
	backDir
};

// ---------------------------------------------------------------------

typedef unsigned long long Vertex;

typedef unordered_set<Vertex> VectorSet; 

short& x(Vertex& v)
{
	short* x = (short*) &v;
	return *x;
}

short& y(Vertex& v)
{
	short* y = (short*) &v;
	y+=1;
	return *y;
}

short& z(Vertex& v)
{
	short* z = (short*) &v;
	z+=2;
	return *z;
}

Vertex vertex(short xV, short yV, short zV)
{
	Vertex v = 0;
	x(v) = xV;
	y(v) = yV;
	z(v) = zV;
	return v;
}

// ---------------------------------------------------------------------

class Face
{
public:
	VectorSet squares;
	vector<Vertex> cornerVertices;
	Orientation orientation;

public:
	void deriveSquares();

private:
	void determineOrientation();
	void collectSquares(Vertex start);
	void tryGoUp(queue<Vertex> &queue, Vertex &v);
	void tryGoDown(queue<Vertex> &queue, Vertex &v);
	void tryGoLeft(queue<Vertex> &queue, Vertex &v);
	void tryGoRight(queue<Vertex> &queue, Vertex &v);

	bool crossesBorder(Vertex from, Vertex to, Direction dir);
};

void Face::determineOrientation()
{
	bool xChanges = false;
	bool yChanges = false;
	bool zChanges = false;
	Vertex v = cornerVertices.front();
	for (vector<Vertex>::iterator iter = cornerVertices.begin(); iter != cornerVertices.end(); iter++)
	{
		xChanges |= x(v) != x(*iter);
		yChanges |= y(v) != y(*iter);
		zChanges |= z(v) != z(*iter);
	}

	if(xChanges && yChanges)
		orientation = zFace;
	if(xChanges && zChanges)
		orientation = yFace;
	if(yChanges && zChanges)
		orientation = xFace;
}

void Face::deriveSquares()
{
	squares.clear();

	determineOrientation();

	vector<Vertex>::iterator iter = cornerVertices.begin();
	Vertex min = *iter;
	while(iter != cornerVertices.end())
	{
		if(*iter < min)
			min = *iter;
		iter++;
	}

	// min = starting point
	squares.insert(min);
	collectSquares(min);
}

void Face::collectSquares(Vertex start)
{
	// try all 4 directions
	queue<Vertex> queue;
	queue.push(start);

	while(!queue.empty())
	{
		Vertex v = queue.front();
		tryGoLeft(queue, v);
		tryGoRight(queue, v);
		tryGoUp(queue, v);
		tryGoDown(queue, v);
		queue.pop();
	}
}

void Face::tryGoLeft(queue<Vertex> &queue, Vertex &v)
{
	Vertex next = v;
	switch(orientation)
	{
		case zFace:
		case yFace:
			x(next)--;
			if(squares.count(next))
				return;
			if(crossesBorder(v, next, leftDir))
				return;
			break;
		case xFace:
			y(next)--;
			if(squares.count(next))
				return;
			if(crossesBorder(v, next, downDir))
			    return;
			break;
	}

	squares.insert(next);
	queue.push(next);
}

void Face::tryGoRight(queue<Vertex> &queue, Vertex &v)
{
	Vertex next = v;
	switch(orientation)
	{
		case zFace:
		case yFace:
			x(next)++;
			if(squares.count(next))
				return;
			if(crossesBorder(v, next, rightDir))
				return;
			break;
		case xFace:
			y(next)++;
			if(squares.count(next))
				return;
			if(crossesBorder(v, next, upDir))
				return;
			break;
	}

	squares.insert(next);
	queue.push(next);
}

void Face::tryGoDown(queue<Vertex> &queue, Vertex &v)
{
	Vertex next = v;
	switch(orientation)
	{
		case zFace:
			y(next)--;
			if(squares.count(next))
				return;
			if(crossesBorder(v, next, downDir))
				return;
			break;
		case xFace:
		case yFace:
			z(next)--;
			if(squares.count(next))
				return;
			if(crossesBorder(v, next, backDir))
				return;
			break;
	}

	squares.insert(next);
	queue.push(next);
}

void Face::tryGoUp(queue<Vertex> &queue, Vertex &v)
{
	Vertex next = v;
	switch(orientation)
	{
		case zFace:
			y(next)++;
			if(squares.count(next))
				return;
			if(crossesBorder(v, next, upDir))
				return;
			break;
		case xFace:
		case yFace:
			z(next)++;
			if(squares.count(next))
				return;
			if(crossesBorder(v, next, frontDir))
				return;
			break;
	}

	squares.insert(next);
	queue.push(next);
}

bool Face::crossesBorder(Vertex from, Vertex to, Direction dir)
{
	Vertex v1 = cornerVertices.back();
	for(vector<Vertex>::iterator iter = cornerVertices.begin(); iter != cornerVertices.end(); iter++)
	{
		Vertex v2 = *iter;
		Vertex v = to;
		switch(dir)
		{
			case leftDir:
				v = from;
			case rightDir:
				if( x(v1) != x(v2) || x(v1) != x(v))
					break;
				switch(orientation)
				{
					case zFace:
						if(y(v1) <= y(v) && y(v) < y(v2) || y(v2) <= y(v) && y(v) < y(v1))
						    return true;
						break;
					case yFace:
						if(z(v1) <= z(v) && z(v) < z(v2) || z(v2) <= z(v) && z(v) < z(v1))
						    return true;
						break;
				}
				break;
			case downDir:
				v = from;
			case upDir:
				if(y(v1) != y(v2) || y(v1) != y(v))
					break;
				switch(orientation)
				{
					case zFace:
						if(x(v1) <= x(v) && x(v) < x(v2) || x(v2) <= x(v) && x(v) < x(v1))
						    return true;
						break;
					case xFace:
						if(z(v1) <= z(v) && z(v) < z(v2) || z(v2) <= z(v) && z(v) < z(v1))
						    return true;
						break;
				}
				break;
			case backDir:
				v = from;
			case frontDir:
				if(z(v1) != z(v2) || z(v1) != z(v))
					break;
				switch(orientation)
				{
					case yFace:
						if(x(v1) <= x(v) && x(v) < x(v2) || x(v2) <= x(v) && x(v) < x(v1))
						    return true;
						break;
					case xFace:
						if(y(v1) <= y(v) && y(v) < y(v2) || y(v2) <= y(v) && y(v) < y(v1))
						    return true;
						break;
				}
				break;
		}
		v1 = v2;
	}
	return false;
}

// ---------------------------------------------------------------------

vector<Face*> faces;
VectorSet blocks;
map<Orientation , VectorSet*> borderSquares;

void tryGoBack(queue<Vertex> &queue, Vertex v)
{
	if(borderSquares[zFace]->count(v))
		return;

	z(v)--;

	if(blocks.count(v))
		return;

	blocks.insert(v);
	queue.push(v);
}

void tryGoFront(queue<Vertex> &queue, Vertex v)
{
	z(v)++;

	if(borderSquares[zFace]->count(v))
		return;

	if(blocks.count(v))
		return;

	blocks.insert(v);
	queue.push(v);
}

void tryGoLeft(queue<Vertex> &queue, Vertex v)
{
	if(borderSquares[xFace]->count(v))
		return;

	x(v)--;

	if(blocks.count(v))
		return;

	blocks.insert(v);
	queue.push(v);
}

void tryGoRight(queue<Vertex> &queue, Vertex v)
{
	x(v)++;

	if(borderSquares[xFace]->count(v))
		return;

	if(blocks.count(v))
		return;

	blocks.insert(v);
	queue.push(v);
}

void tryGoDown(queue<Vertex> &queue, Vertex v)
{
	if(borderSquares[yFace]->count(v))
		return;

	y(v)--;

	if(blocks.count(v))
		return;

	blocks.insert(v);
	queue.push(v);
}

void tryGoUp(queue<Vertex> &queue, Vertex v)
{
	y(v)++;

	if(borderSquares[yFace]->count(v))
		return;

	if(blocks.count(v))
		return;

	blocks.insert(v);
	queue.push(v);
}

void collectBlocks(Vertex start)
{
	queue<Vertex> queue;
	blocks.insert(start);
	queue.push(start);

	while(!queue.empty())
	{
		Vertex v = queue.front();
		tryGoLeft(queue, v);
		tryGoRight(queue, v);
		tryGoDown(queue, v);
		tryGoUp(queue, v);
		tryGoBack(queue, v);
		tryGoFront(queue, v);
		queue.pop();
	}
}

void solve()
{
	borderSquares[xFace]->clear();
	borderSquares[yFace]->clear();
	borderSquares[zFace]->clear();

	for (vector<Face*>::iterator iter = faces.begin(); iter != faces.end(); iter++)
	{
		// derive all Squares of this face
		Face* f = *iter;
//		unsigned int startTime = getMicroSecs();
//		cout << "deriveSquares: ";
		f->deriveSquares();
//		cout << (getMicroSecs() - startTime) << endl;

		// put all squares of faces with the same orientation together
		VectorSet* squares = borderSquares[f->orientation];
		for(VectorSet::iterator iter = f->squares.begin(); iter != f->squares.end(); iter++)
		{
			Vertex v = *iter;
			squares->insert(v);
		}
	}

	VectorSet::iterator iter = borderSquares[zFace]->begin();
	Vertex min = *iter;
	while(iter != borderSquares[zFace]->end())
	{
		if(*iter < min)
			min = *iter;
		iter++;
	}

	blocks.clear();
	blocks.insert(min);
//	unsigned int startTime = getMicroSecs();
//		cout << "collectBlocks: ";
		collectBlocks(min);
//		cout << (getMicroSecs() - startTime) << endl;

	
	cout << "The bulk is composed of " << blocks.size() << " units.\n";
}

int main(int argc, char** args)
{
	borderSquares[xFace] = new VectorSet();
	borderSquares[yFace] = new VectorSet();
	borderSquares[zFace] = new VectorSet();

	int numTests;
	istream* input = &cin;
	if(argc > 1)
		input = new ifstream("/work/workspace/SphereOnlineJudge/testcases/BULK.txt");
	(*input) >> numTests;
	while(numTests-->0)
	{
		faces.clear();
		int numFaces;
		(*input) >> numFaces;
		while(numFaces-->0)
		{
			int numVertices;
			(*input) >> numVertices;
			Face *face = new Face();
			while(numVertices-->0)
			{
				short xV,yV,zV;
				(*input) >> xV >> yV >> zV;
				face->cornerVertices.push_back(vertex(xV, yV, zV));
			}
			faces.push_back(face);
		}
		solve();

		for(vector<Face*>::iterator iter = faces.begin(); iter != faces.end(); iter++)
			delete *iter;
	}
}
