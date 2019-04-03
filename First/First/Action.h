#pragma once

#include "Point2D.h"
#include <iostream>

class Warrior;
using namespace std;


class Action
{
public:
	enum eType { RUN, FIND_AMMO, FIND_MED, FIGHT };
	
	Action(Warrior& warrior, eType type);
	Action &operator=(const Action & other) = delete;

	void updateScore();
	int getScore() const { return score; };
	Warrior& getWarrior() const;
	eType getType() const;
	
	
private:
	static const int NUM_OF_TYPES = 4;
	const double MAX_SCORE = 100;


	Warrior& warrior;
	double score;
	eType type;


	double points_Calculation(Action::eType type, Warrior &warrior);
	double calculate_points_Run_Mission(Warrior& w);
	double calculate_points_Find_Ammo_Mission(Warrior& w);
	double calculate_points_Find_Med_Mission(Warrior& w);
	double calculate_Points_Fight_Mission(Warrior& w);
	double calculate_Points_Hurt_Mission(Warrior& w);

public:
};