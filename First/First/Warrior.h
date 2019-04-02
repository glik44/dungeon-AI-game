#pragma once

#include "Point2D.h"
#include "Room.h"
#include "Node.h"
#include <queue>
#include <vector>
#include <stack>
#include <iostream>
#include "CompareNodes.h"
#include "CompareActions.h"
#include "ConstValue.h"
#include "Parent.h"
#include "Action.h"
#include "Maze.h"

class Maze;
class Point2D;
using namespace std;

/*
The warrior need to implements 2 praiorty Q
to select a mission
and for nevigation.
*/
class Warrior
{
private:
	//const values.
	const int MAX_LIFE = 100;
	const int MAX_GUNS_AMMO = 50;
	const int MAX_GRANDE_AMMO = 2;

	//variables.
	Maze* maze;

	int		id;
	int		lifePoint = MAX_LIFE; //range [0,100]
	int		gunsAmmo = MAX_GUNS_AMMO;
	int		grenadeAmmo = MAX_GRANDE_AMMO; 
	int		safetyScore; // Depends on walls neer the warrior 

	// personal data
	double ammoP, medP, fightP;

	bool	life = true;
	bool	isInRoom = true;
	Room	*currentRoom;
	Point2D	location;

	stack<Point2D> walkingPath;
	priority_queue<Action*, vector<Action*>, CompareActions> actionQueue;

	Action *currentAction;

	//functions
	void exitTheRoom(Room &room); //Implaments in A*.
	double Warrior::getDistance(const Point2D &p1, const Point2D &p2) const;
	double getDistance(const Warrior &other) const;
	void lookForEnemyInRoom(Warrior &other);
	void lookForAmmo();

	void shoot(Warrior &other);
	void throGrenade(Warrior &other);
	void calculateVactorValues(double &Vx, double &Vy,const Point2D &p);
	void NormalizingVector(double &Vx, double &Vy);
	void checkStorage(Action::eType action);
	Point2D& getTargetByVector(Room &room, double &Vx, double &Vy);
public:
	Warrior(Room &room, Point2D &location, double ammoP, double medP, double fightP);
	~Warrior();

	void selectMission(Warrior& other);
	void moveWarrior(Point2D &nextStep);

	void lookForStorage(Storage &s, bool ammo);
	void lookForEnemy(Warrior &other);
	void lookForMedicalStorage();
	void updateCurrentRoom();
	void injured(double hitPoint);

	//getters & setters
	int getlifePoints() { return lifePoint; }
	int	getGunsAmmo() { return gunsAmmo; }
	int	getGrenadeAmmo() { return grenadeAmmo; }
	Room* getCurrentRoom() { return currentRoom; }
	Point2D getLocation() const { return location; }
	double getMaxLife() const { return MAX_LIFE; }
	double getMaxGuns() const { return MAX_GUNS_AMMO; }
	double getMaxGrandes() const { return MAX_GRANDE_AMMO; }
	double getMedP() const { return medP; }
	double getAmmoP() const { return ammoP; }
	double getFightP() const { return fightP; }
	void setX(int x) { this->location.setX(x); }
	void setY(int y) { this->location.setY(y); }
	bool isAlive() const { return life; }
	void updateActions();
	bool canFight(Warrior &other) const;

};

