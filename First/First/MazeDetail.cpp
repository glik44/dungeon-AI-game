#include "MazeDetail.h"



Maze_Detail::~Maze_Detail()
{

}

Maze_Detail::Maze_Detail(Point2D* location, int type, double saftyScore) :location(location), type(type), saftyScore(saftyScore)
{
}

void Maze_Detail::reset_Type()
{
	type = originType;
}
