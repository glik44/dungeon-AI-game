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
#include "Definitions.h"
#include "Parent.h"
#include "Action.h"
#include "Maze.h"

class Maze;
class Point2D;
using namespace std;


class Warrior
{
private:
	//variables.
	Maze* maze;

	int		id;
	int		lifePoint = MAX_LIFE; //range [0,100]
	int		gunsAmmo = MAX_GUNS_AMMO;
	int		grenadeAmmo = MAX_GRANDE_AMMO; 
	int		safetyScore; // calculated by the number of objects neer the worrior


	double ammoP, medP, fightP;

	bool	life = true;
	bool	isInRoom = true;
	Room	*currentRoom;
	Point2D	location;

	stack<Point2D> walkingPath;
	priority_queue<Action*, vector<Action*>, CompareActions> actionQueue;

	Action *currentAction;

	//functions
	void exit_Room(Room &room); 
	double Warrior::getDistance(const Point2D &p1, const Point2D &p2) const;
	double getDistance(const Warrior &other) const;
	void find_Enemy_In_Room(Warrior &other);
	void lookForAmmo();
	void check_warrior_loop(Warrior &other);

	void shoot(Warrior &other);
	void throw_Grenade(Warrior &other);
	void calculate_Vactor_Values(double &Vx, double &Vy,const Point2D &p);
	void Normalizing_Vector(double &Vx, double &Vy);
	void check_Storage(Action::eType action);
	Point2D& get_Target_By_Vector(Room &room, double &Vx, double &Vy);
public:
	Warrior(Room &room, Point2D &location, double ammoP, double medP, double fightP);
	~Warrior();

	void select_Mission(Warrior& other);
	void move_Warrior(Point2D &nextStep);

	void find_Storage(Storage &s, bool ammo);
	void find_Enemy(Warrior &other);
	void lookForMedicalStorage();
	void update_Current_Room();
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
	void update_Actions_Queue();
	bool check_Fight(Warrior &other) const;

};

