#pragma once
#include "Point2D.h"
#include "Room.h"
#include <stdlib.h>
#include <time.h>

/*the storage activity
increasing life point OR ammo implements in the player class
The storage size is 3X3 by defult.
*/
class Storage
{
private:
	Point2D *location; // use like ID and for draw.
	Room *room; 
	int type;

	//functions
	//void setCenterByRoom();
public:
	static const int STORAGE_SIZE = 3;
	
	//functions
	Storage(Room &containedInRoom, Point2D &location, int type);
	Storage() {};
	~Storage();
	Point2D &getLocation() const;
	Room &getRoom() const { return *room; }
	int getType() const;
};

