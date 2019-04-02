#pragma once
#include "Point2D.h"
#include "Definitions.h"

class MazeDetail
{
private:
	Point2D *location;
	double saftyScore;
	int type;
	int originType;

public:
	~MazeDetail();
	MazeDetail(Point2D *location = nullptr, int type = SPACE, double saftyScore = 0);

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
