#pragma once
#include "Point2D.h"
#include "Room.h"
#include <stdlib.h>
#include <time.h>


class Storage
{
private:
	Point2D *location; 
	Room *room; 
	int type;

public:
	static const int STORAGE_SIZE = 3;
	
	Storage(Room &containedInRoom, Point2D &location, int type);
	Storage() {};
	~Storage();
	Point2D &getLocation() const;
	Room &getRoom() const { return *room; }
	int getType() const;
};

