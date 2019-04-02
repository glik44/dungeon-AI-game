#pragma once
#include "StorageNode.h"
#include "Maze.h"

class CompareStorages
{
public:
	CompareStorages();
	~CompareStorages();
	bool operator( ) (const StorageNode* n1, const StorageNode* n2);
};

