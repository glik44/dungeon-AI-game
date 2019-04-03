#pragma once
#include "Action.h"
#include "Maze.h"

class Warrior;

class CompareActions
{
private:
	const double MAX_SCORE = 100;


public:
	CompareActions();
	~CompareActions();
	bool operator( ) (Action* a1, Action* a2);
};

