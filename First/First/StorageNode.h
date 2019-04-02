#pragma once

#include "Storage.h"
class StorageNode
{
public:
	StorageNode();
	~StorageNode();
private:
	Storage *s;
	Point2D *current, *enamy;
	double g;
public:
	double GetF() const;
	double GetG() const;
	double GetH() const;
	StorageNode(Point2D& c, Point2D& e, Storage &s);
	Storage &getStorage() const;
	Point2D &getEnamy() const;
	Point2D &getCurrent() const;
};

