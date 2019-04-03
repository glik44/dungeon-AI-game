#include "Room.h"
#include "Door.h"
#include "Maze.h"
#include <stdlib.h>

Room::Room()
{
}


Room::~Room()
{
}


Room::Room(int id, const Point2D& center_point, int w, int h)
{
	this->id = id;
	center = (Point2D* )&center_point;
	width = w;
	height = h;
}


Point2D& Room::GetCenter() const
{
	return *center;
}


int Room::GetWidth() const
{
	return width;
}


int Room::GetHeight() const
{
	return height;
}


bool Room::IsOverlap(const Room& other)
{
	return abs(center->GetX()-other.GetCenter().GetX())
		< (width+other.width)/2+5 && abs(center->GetY() - 
			other.GetCenter().GetY()) < (height+other.height)/2+5  ;
}

int Room::getLeft() const
{
	int left = this->GetCenter().GetX() - this->GetWidth() / 2;
	if (left < 0)
		return 0;
	else
		return left;
}

int Room::getRight() const
{
	int right = this->GetCenter().GetX() + this->GetWidth() / 2;
	if (right >= MSIZE)
		return MSIZE - 1 ;
	return right;
}

int Room::getTop() const
{
	int top = this->GetCenter().GetY() - this->GetHeight() / 2;
	if(top < 0)
		return 0;
	else 
		return top;
}

int Room::getBottom() const
{
	int bottom = this->GetCenter().GetY() + this->GetHeight() / 2;
	if (bottom >= MSIZE) return bottom = MSIZE - 1;
	else
		return bottom;
}

vector<Door*> Room::getDoors() const
{
	return doors;
}

void Room::addDoor(Door& door)
{
	doors.push_back(&door);
}


void Room::currectPointToBeInRoom(Point2D & p)
{
	if (p.GetX() < this->getLeft())
		p.setX(this->getLeft());
	else if (p.GetX() > this->getRight())
		p.setX(this->getRight());
	if (p.GetY() < this->getTop())
		p.setY(this->getTop());
	else if (p.GetY() < this->getBottom())
		p.setY(this->getBottom());
}

bool Room::locatedInTheRoom(const Point2D & p) const
{
	int x = p.GetX();
	int y = p.GetY();

	int top = center->GetY() + (height / 2);
	int bottom = center->GetY() - (height / 2);
	int left = center->GetX() - (width / 2);
	int right = center->GetX() + (width / 2);
	
	if (y <= top && y >= bottom && x >= left && x <= right)
		return true;

	return false;
}


bool Room::isDestionationRoom(const Room & destionation) const
{
	int numOfDoors = doors.size();
	for (int i = 0; i < numOfDoors; i++)
	{
		if (doors[i]->isDestinationDoor(destionation) )
			return true;
	}
	return false;
}

Point2D& Room::getRandomPointInRoom()
{	
	srand(time(0));

	int posX = rand() % 2 == 1 ? 1 : -1;
	int posY = rand() % 2 == 1 ? 1 : -1;

	int x = center->GetX() + posX * (rand() % (width / 2));
	int y = center->GetY() + posY * (rand() % (height / 2));

	return *new Point2D(x, y);
}
