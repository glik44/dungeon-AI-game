#include "Warrior.h"
#include "Action.h"
#include "Door.h"
#include "CompareActions.h"

/**
ToDo:
- search for ammo&med
- run away
- shoot
*/
Warrior::Warrior(Room &room, Point2D &location, double ammoP, double medP, double fightP) :
	currentRoom(&room), location(location),ammoP(ammoP), medP(medP), fightP(fightP)
{
	maze = &Maze::getInstance();
	static int genId = 0;
	this->id = genId++;
	updateActions();
}

Warrior::~Warrior()
{
	// delete actionQueue
	// ..
}

/*go to the next room to reach the destination room */
void Warrior::exitTheRoom(Room &destRoom)
{
	//1. serch the door with the destionation.
	vector<Door*> doors = currentRoom->getDoors();
	int numOfDoors = doors.size();
	Door *nextDoor = nullptr;
	for (int i = 0; i < numOfDoors; i++)
	{
		if (doors[i]->isDestinationDoor(destRoom))
		{
			nextDoor = doors[i];
			break;
		}
	}


	walkingPath = maze->localAStar(location, nextDoor->getExitLocation());

	cout << "currentRoom: x: " << currentRoom->GetCenter().GetX() << " y: " << currentRoom->GetCenter().GetY() << endl;

	cout << "currentRoom: x: " << currentRoom->GetCenter().GetX() << " y: " << currentRoom->GetCenter().GetY() << endl;
}


double Warrior::getDistance(const Point2D &p1, const Point2D &p2) const
{
	int x = p1.GetX() - p2.GetX();
	int y = p1.GetY() - p2.GetY();

	return sqrt(pow(x, 2) + pow(y, 2));
}

double Warrior::getDistance(const Warrior & other) const
{
	return getDistance(this->getLocation(), other.getLocation());
}
void Warrior::checkStorage(Action::eType action)
{
	if (action == Action::FIND_AMMO && maze->checkIfPointIsAmmoStorage(this->getLocation()))
	{
		this->gunsAmmo = MAX_GUNS_AMMO;
		this->grenadeAmmo = MAX_GRANDE_AMMO;
		cout << "Warrior" << this->id << "get AMMO! ;) " << endl;
	}
	else if (action == Action::FIND_MED && maze->checkIfPointIsMedStorage(this->getLocation()))
	{
		this->lifePoint = MAX_LIFE;
		cout << "Warrior" << this->id << "get HEALED :)" << endl;
	}
}


/*Warrior select a mission.
if the warrior started to walk he keep going
else he select new mission.*/
void Warrior::selectMission(Warrior& other)
{
	if (this->lifePoint <= 50)
		cout << "Test" << endl;
	static int count = 0;
	srand(time(0));

	if (currentRoom != nullptr && getDistance(other) < SHOOT_MAX_DISTANCE && (other.getCurrentRoom() == currentRoom))
	{
		if (currentAction != nullptr && currentAction->getType() == Action::FIGHT)
			while (!walkingPath.empty())
				walkingPath.pop();
		shoot(other);

	}
	if (walkingPath.size() > 0)
	{
		moveWarrior(walkingPath.top());
		walkingPath.pop();
	}
	else
	{
		count++;
		currentAction = actionQueue.top();	// RUN, FIND_AMMO, FIND_MED, FIGHT
		checkStorage(currentAction->getType());


		switch (currentAction->getType())
		{
		case Action::FIGHT:
			lookForEnemy(other);
			break;
		case Action::RUN:
			
		case Action::FIND_MED:
			lookForStorage(maze->getTargetStorage(Action::FIND_MED, location, other.getLocation()), false);
			break;
		case Action::FIND_AMMO:
			lookForStorage(maze->getTargetStorage(Action::FIND_AMMO, location, other.getLocation()), true);
			break;
		}
		updateActions();
	}
}
void Warrior::updateActions()
{
	while (!actionQueue.empty())
	{
		actionQueue.pop();
	}

	actionQueue.push(new Action(*this, Action::FIGHT));
	actionQueue.push(new Action(*this, Action::FIND_AMMO));
	actionQueue.push(new Action(*this, Action::RUN));
	actionQueue.push(new Action(*this, Action::FIND_MED));
}

bool Warrior::canFight(Warrior & other) const
{
	return (currentRoom != nullptr && (other.getCurrentRoom()) != nullptr) && ((currentRoom->getId()) == (other.getCurrentRoom()->getId()));
}

/*Serch enemy in the maze
look in the current room, then go to another room and check him.*/
void Warrior::lookForEnemy(Warrior &other)
{
	if (currentRoom != nullptr && other.getCurrentRoom() != nullptr && currentRoom->getId() != other.getCurrentRoom()->getId())
		exitTheRoom(*other.getCurrentRoom());

	else
		lookForEnemyInRoom(other); 
}

/*Serch enemy in the maze
look in the current room, then go to another room and check him.*/
void Warrior::lookForStorage(Storage &s, bool ammo)
{
	cout << "looking  for " ;
	if (ammo)
		cout << " ammo" << endl;
	else
		cout << " med" << endl;


	//1. look in the current room.
	if (currentRoom != nullptr && currentRoom->getId() != s.getRoom().getId())
		exitTheRoom(s.getRoom());
	else if (ammo)
		walkingPath = maze->localAStar(location, s.getLocation()); // enemy is in the room
	else
		walkingPath = maze->localAStar(location, s.getLocation());
}

/*
Look for other warrior in room using A* algorithm
use stack::walkingPath to save the steps and move the warrior.
*/
void Warrior::lookForEnemyInRoom(Warrior &other)
{
	if (canFight(other))
	{
		if (getDistance(other) > SHOOT_MAX_DISTANCE && this->grenadeAmmo > 0)
			this->throGrenade(other);
		else if (getDistance(other) < SHOOT_MAX_DISTANCE)
				shoot(other);
	}
	else
		walkingPath = maze->localAStar(location, other.getLocation());
}

/*Get a point and move the warrior on the maze to the point cordinate*/
void Warrior::moveWarrior(Point2D &nextStep)
{
	//deleate the warrior from the maze
	maze->parts[location.GetY()][location.GetX()].resetType();

	//change the location of warrior.
	location.setX(nextStep.GetX());
	location.setY(nextStep.GetY());

	//draw the warrior on the maze
	maze->parts[location.GetY()][location.GetX()].setType(WARRIOR);
	updateCurrentRoom();
}


/*
Each shot consumes one ball.
The damage caused to the second fighter depends on the distance between them
*/
void Warrior::shoot(Warrior &other)
{
	srand(time(0));
	int hit = rand() % 10;

	if (hit < 3)
	{
		cout << "Warrior " << this->id << " Miss the shoot" << endl;
		return;
	}

	if (gunsAmmo <= 0)
		return;

	double distance = getDistance(other);

	cout << "warrior " << id << " is trying to soot" << endl;
	//check if the warrior no too far
	int damage = (SHOOT_MAX_DISTANCE - (int)distance);
	damage *= 5;
	if (damage > 0)
	{
		currentAction->updateScore();
		gunsAmmo--;
		cout << this->id << " shot! " << other.id <<"("<<damage <<")" <<endl;
		other.injured(damage);
	}
	else
		walkingPath = maze->localAStar(location, other.getLocation());
}

/* Decrease the life point until dead. */
void Warrior::injured(double hitPoint)
{
	// TODO: update action med and action run priority
	
	lifePoint = lifePoint - ((1-safetyScore/MAX_SAFTY_SCORE) * hitPoint);
	if (lifePoint <= 0)
	{
		lifePoint = 0;
		life = false;
		cout << "Warrior " << this - id << " died" << endl;
	}
}

void Warrior::throGrenade(Warrior & other)
{
	if (currentRoom == nullptr)
		return;
	//1. Variables 
	const double grenadeMaxDamage = 30;
	double damage = 0;
	double Vx, Vy; // vectors values
	Point2D targetLocation;
	//2.  throw the grenade 
	//2.1. calculate the vector to knoe the direction for throwing the grenade
	calculateVactorValues(Vx, Vy, other.getLocation());
	//2.2. Normalizing the vector and adapting it to the warrior's ability to throw.
	NormalizingVector(Vx, Vy);
	//2.2. calculate the target location.
	targetLocation = getTargetByVector(*currentRoom, Vx, Vy);
	//3. Calculate damage
	damage = (ConstValue::GRENADE_DEMAGE_RADIOS - getDistance(targetLocation, other.getLocation())) 
		/ ConstValue::GRENADE_DEMAGE_RADIOS * grenadeMaxDamage;
	//4. Trow the grenade
	cout << "Warrior " << this->id << "trow grenade!" << endl;
	if (damage > 0)
		other.injured(damage);
}

/*insert to Vx and Vy the values of the calculate vector*/
void Warrior::calculateVactorValues(double & Vx, double & Vy, const Point2D & p)
{
	Vx = p.GetX() - this->getLocation().GetX();
	Vy = p.GetY() - this->getLocation().GetY();
}

/*Normalizing the values of the vactor*/
void Warrior::NormalizingVector(double & Vx, double & Vy)
{
	double vectorSize = sqrt((Vx*Vx) + (Vy*Vy));
	Vx = Vx / vectorSize;
	Vy = Vy / vectorSize;
}

/*get the direction by the vector and calculate the target point*/
Point2D& Warrior::getTargetByVector(Room & room, double & Vx, double & Vy)
{
	int x, y;
	Point2D* p;
	x = (int)(Vx * ConstValue::TROW_GRENADE_MAX_DISTANCE) + this->getLocation().GetX();
	y = (int)(Vy * ConstValue::TROW_GRENADE_MAX_DISTANCE) + this->getLocation().GetY();
	p = new Point2D(x, y);
	if (room.locatedInTheRoom(*p))
		return *p;
	else
		room.currectPointToBeInRoom(*p);
	return *p;
}

void Warrior::updateCurrentRoom()
{
	Room* rooms = maze->getRooms();

	for (int i = 0; i < NUM_ROOMS; i++)
	{
		if (rooms[i].locatedInTheRoom(location))
		{
			currentRoom = &rooms[i];
			return;
		}
	}
	currentRoom = nullptr;
}