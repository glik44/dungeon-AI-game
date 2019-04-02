#pragma once
#include "Point2D.h"

class MazePart
{
public:
	static const int SPACE = 1;
	static const int WALL = 2;
	static const int VISITED = 3;
	static const int TARGET = 5;
	static const int GRAY = 6;
	static const int MEDICAL = 7;
	static const int AMMO = 8;
	static const int WARRIOR = 9;
	static const int DOOR = 10;

private:
	Point2D *location;
	double saftyScore;
	int type;
	int originType;

public:
	~MazePart();
	MazePart(Point2D *location = nullptr, int type = SPACE, double saftyScore = 0);

	Point2D& getLocation() const { return *location; }
	double getSaftyScore() const { return saftyScore; }
	int getType() const { return type; }
	int getOriginType() const { return originType; }
	void setLocation(Point2D &location) { this->location = &location; }
	void setSaftyScore(double saftyScore) { this->saftyScore = saftyScore; }
	void setType(int type) { this->type = type; }
	void setOriginType(int type) { this->originType = type; }
	void resetType();
};
