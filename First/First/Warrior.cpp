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


Warrior::Warrior(Room &room, Point2D &location) :
	currentRoom(&room), location(location)
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

	//2. go to the enter of door.
	//Point2D &p = doorDest->getEnterLocation();
	//this->localAStar(p);
	//3. got to the exit of the door.

	walkingPath = maze->localAStar(location, nextDoor->getExitLocation());

	cout << "currentRoom: x: " << currentRoom->GetCenter().GetX() << " y: " << currentRoom->GetCenter().GetY() << endl;
	/*vector<Room*> v = nextDoor->getDestinations();
	currentRoom = v.front();*/
	cout << "currentRoom: x: " << currentRoom->GetCenter().GetX() << " y: " << currentRoom->GetCenter().GetY() << endl;
}

double Warrior::getDistance(const Warrior & other) const
{
	int x = this->location.GetX() - other.location.GetX();
	int y = this->location.GetY() - other.location.GetY();

	return sqrt(pow(x, 2) + pow(y, 2));
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

	if (currentRoom != nullptr && getDistance(other) < SHOOT_MAX_DISTANCE && (&other.getCurrentRoom() == currentRoom))
	{
		if (currentAction->getType() == Action::FIGHT)
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

		lookForEnemy(other);
		switch (currentAction->getType())
		{
		case Action::FIGHT:
			lookForEnemy(other);
			break;
		case Action::RUN:
			// runAway();
			break;
		case Action::FIND_AMMO:
			lookForStorage(maze->getClosestStorage(Action::FIND_AMMO, location), true);
			break;
		case Action::FIND_MED:
			lookForStorage(maze->getClosestStorage(Action::FIND_MED, location), false);
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

/*Serch enemy in the maze
look in the current room, then go to another room and check him.*/
void Warrior::lookForEnemy(Warrior &other)
{
	//1. look for enemy in the current room.
	if (currentRoom != nullptr && &other.getCurrentRoom() != nullptr && currentRoom->getId() != other.getCurrentRoom().getId())
		exitTheRoom(other.getCurrentRoom());
	else
		lookForEnemyInRoom(other); // enemy is in the room

								   //2. calculat the next room the warrior will check.

								   //3. go to next room  safely as possible.

								   //4. look for enemy in the current room.
}

/*Serch enemy in the maze
look in the current room, then go to another room and check him.*/
void Warrior::lookForStorage(Storage &s, bool ammo)
{
	cout << "looking  for ";
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
	if (getDistance(other) < SHOOT_MAX_DISTANCE)
		shoot(other);
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
	maze->parts[location.GetY()][location.GetX()].setType(MazePart::WARRIOR);
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

	if (hit < 7)
		return;

	if (gunsAmmo <= 0)
		return;
	//Check the ammo.
	double distance = getDistance(other);

	cout << "warrior " << id << " is trying to soot" << endl;
	//check if the warrior no too far
	int damage = SHOOT_MAX_DISTANCE - (int)distance;
	if (damage > 0)
	{
		currentAction->updateScore();
		gunsAmmo--;
		cout << this->id << " shot! " << other.id << endl;
		other.injured(damage);
	}
	else
		walkingPath = maze->localAStar(location, other.getLocation());
}

/* Decrease the life point until dead. */
void Warrior::injured(int hitPoint)
{
	// TODO: update action med and action run priority

	lifePoint = lifePoint - hitPoint;
	if (lifePoint <= 0)
	{
		lifePoint = 0;
		life = false;
	}
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