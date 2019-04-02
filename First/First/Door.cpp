#include "Door.h"

Door::Door(Room& current, Point2D &enterlocation, Point2D &exitlocation):
	current(current), enterLocation(enterlocation), exitLocation(exitlocation)
{
	current.getBottom();
}


Door::~Door()
{
}

/*Check if we allrady visit in the destination of the door.
visit? pass the door. else serch.
*/
bool Door::isDestinationDoor(const Room& destination) const
{
	for (int i = 0; i < destinations.size(); i++)
	{
		if (destinations[i]->GetCenter().GetX() == destination.GetCenter().GetX() 
			&& destinations[i]->GetCenter().GetY() == destination.GetCenter().GetY())
			return true;
	}
	/*if ( this->destinations.find((Room*)&destination) != destinations.end() )
	{
		return true;
	}*/
	return false;

}

