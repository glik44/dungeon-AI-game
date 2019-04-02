#pragma once
#include "Action.h"
#include "Maze.h"

class Warrior;

class CompareActions
{
private:
	const double MAX_SCORE = 100;//calculate function return value in the range [0,100]

	///*Calculate the Score by the action type 
	//and the parameters we can get from the warrior and need for the calculate function.*/
	//double calculateScore(Action::eType type, Warrior &warrior);
	//double calculateScoreRun(Warrior& w);
	//double calculateScoreFindAmmo(Warrior& w);
	//double calculateScoreFindMed(Warrior& w);
	//double calculateScoreFight(Warrior& w);
	//double calculateScoreHurt(Warrior& w);

public:
	CompareActions();
	~CompareActions();
	bool operator( ) (Action* a1, Action* a2);
};

