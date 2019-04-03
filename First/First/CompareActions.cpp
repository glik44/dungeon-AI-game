#include "CompareActions.h"
#include "Warrior.h"


CompareActions::CompareActions()
{
}


CompareActions::~CompareActions()
{
}



bool CompareActions::operator( )(Action* a1, Action* a2)
{
	return a1->getScore() < a2->getScore();
}