#pragma once
#include "Point2D.h"
#include "ConstValue.h"
#include <math.h>
<<<<<<< HEAD:My_Room.h
#include "Door.h"
class My_Room
=======
#include <vector>
#include <set>

class Door;

using namespace std;

class Room
>>>>>>> 11c7df58eb385081522b14958d27f8239946ab4d:Room.h
{
public:
	My_Room();
	~My_Room();
	static const int MINIMUM_SIZE = 3;
private:
	int id;
	Point2D *center;
	vector<Door*> doors;

	int width, height;
<<<<<<< HEAD:My_Room.h
	std::vector<Door> doors;

public:
	My_Room(const Point2D& center_point, int w, int h);
	Point2D GetCenter() const;
	int GetWidth() const;
	int GetHeight() const;
	bool IsOverlap(const My_Room& other) const;
	bool locatedInTheRoom(const Point2D &p) const;
	void addDoor(Door door);
	std::vector<Door> getAllDoors();

=======
public:
	Room(int id, const Point2D& center_point, int w, int h);
	Point2D &GetCenter() const;
	int GetWidth() const;
	int GetHeight() const;
	bool IsOverlap(const Room& other);
	bool locatedInTheRoom(const Point2D &p) const;
	bool isDestionationRoom(const Room &destionation) const;
	
>>>>>>> 11c7df58eb385081522b14958d27f8239946ab4d:Room.h
	//get the coordinate boundary of the room.
	int getId() { return id; }
	int getLeft() const;
	int getRight() const;
	int getTop() const;
	int getBottom() const;
	vector<Door*> getDoors() const;
	void addDoor(Door& door);

	

	
};

