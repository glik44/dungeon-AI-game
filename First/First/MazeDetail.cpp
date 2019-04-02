#include "MazeDetail.h"



MazeDetail::~MazeDetail()
{

}

MazeDetail::MazeDetail(Point2D* location, int type, double saftyScore) :location(location), type(type), saftyScore(saftyScore)
{
}

void MazeDetail::resetType()
{
	type = originType;
}
