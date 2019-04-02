#include "CompareNodes.h"



CompareNodes::CompareNodes()
{
}


CompareNodes::~CompareNodes()
{
}


bool CompareNodes::operator ( )(const Node* n1, const Node* n2)
{
	Point2D &p1 = n1->GetPoint();
	Point2D &p2 = n2->GetPoint();

	int x1 = p1.GetX(), y1 = p1.GetY(), x2 = p2.GetX(), y2 = p2.GetY();
	Maze &maze = Maze::getInstance();
	double safty1 = maze.parts[y1][x1].getSaftyScore();
	double safty2 = maze.parts[y2][x2].getSaftyScore();

	return n1->GetF()+(1-safty1) > n2->GetF()+(1-safty2);
}
