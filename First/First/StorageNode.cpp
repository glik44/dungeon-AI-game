#include "StorageNode.h"
#include <math.h>



StorageNode::StorageNode()
{
	g = 0;
}


StorageNode::~StorageNode()
{
}


double StorageNode::GetF() const
{
	return GetH() - GetG();
}

// calc storage distance from enamy
double StorageNode::GetG() const
{
	// the bigget the distance the better
	int enamyX = enamy->GetX();
	int enamyY = enamy->GetY();
	return sqrt(pow(s->getLocation().GetX() - enamyX, 2) + pow(s->getLocation().GetY() - enamyY, 2));
	return g;
}

// calc distance from storage
double StorageNode::GetH() const
{
	return sqrt(pow(current->GetX() - s->getLocation().GetX(), 2) +
		pow(current->GetY() - s->getLocation().GetY(), 2));
}

StorageNode::StorageNode(Point2D& c, Point2D& e, Storage &s): 
						current(&c), enamy(&e), s(&s), g(0)
{
}


Storage & StorageNode::getStorage() const
{
	return *s;
}
Point2D & StorageNode::getEnamy() const
{
	return *enamy;
}
Point2D & StorageNode::getCurrent() const
{
	return *current;
}
