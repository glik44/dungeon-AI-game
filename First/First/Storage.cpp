#include "Storage.h"


Storage::Storage(Room &containedInRoom, Point2D &location, int type): room(&containedInRoom), location(&location), type(type)
{

}


Storage::~Storage()
{
}

Point2D &Storage::getLocation() const
{
	return *location;
}

int Storage::getType() const
{
	return type;
}