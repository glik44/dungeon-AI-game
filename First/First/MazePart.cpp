#include "MazePart.h"



MazePart::~MazePart()
{
	int x = 5;
}

MazePart::MazePart(Point2D* location, int type, double saftyScore) :location(location), type(type), saftyScore(saftyScore)
{
}

void MazePart::resetType()
{
	type = originType;
}
