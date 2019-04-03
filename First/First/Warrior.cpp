#include "Warrior.h"
#include "Action.h"
#include "Door.h"
#include "CompareActions.h"



Warrior::Warrior(Room &room, Point2D &location, double ammoP, double medP, double fightP) :
	currentRoom(&room), location(location),ammoP(ammoP), medP(medP), fightP(fightP)
{
	maze = &Maze::getInstance();
	static int genId = 0;
	this->id = genId++;
	update_Actions_Queue();
}

Warrior::~Warrior()
{
	// delete actionQueue
	// ..
}


void Warrior::exit_Room(Room &destRoom)
{
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


	walkingPath = maze->local_A_Star(location, nextDoor->getExitLocation());

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

void Warrior::check_Storage(Action::eType action)
{
	if (action == Action::FIND_AMMO && maze->Is_Point_Is_Ammo_Storage(this->getLocation()))
	{
		this->gunsAmmo = MAX_GUNS_AMMO;
		this->grenadeAmmo = MAX_GRANDE_AMMO;
		cout << "Warrior" << this->id << "got Armed! " << endl;
	}
	else if (action == Action::FIND_MED && maze->Is_Point_Is_Med_Storage(this->getLocation()))
	{
		this->lifePoint = MAX_LIFE;
		cout << "Warrior" << this->id << "got HEALED" << endl;
	}
}


void Warrior::select_Mission(Warrior& other)
{
	
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
		move_Warrior(walkingPath.top());
		walkingPath.pop();
	}
	else
	{
		count++;
		currentAction = actionQueue.top();	
		check_Storage(currentAction->getType());


		switch (currentAction->getType())
		{
		case Action::FIGHT:
			find_Enemy(other);
			break;
		case Action::RUN:
			
		case Action::FIND_MED:
			find_Storage(maze->getTargetStorage(Action::FIND_MED, location, other.getLocation()), false);
			break;
		case Action::FIND_AMMO:
			find_Storage(maze->getTargetStorage(Action::FIND_AMMO, location, other.getLocation()), true);
			break;
		}
		update_Actions_Queue();
	}
}
void Warrior::update_Actions_Queue()
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

bool Warrior::check_Fight(Warrior & other) const
{
	return (currentRoom != nullptr && (other.getCurrentRoom()) != nullptr) && ((currentRoom->getId()) == (other.getCurrentRoom()->getId()));
}


void Warrior::find_Enemy(Warrior &other)
{
	if (currentRoom != nullptr && other.getCurrentRoom() != nullptr && currentRoom->getId() != other.getCurrentRoom()->getId())
		exit_Room(*other.getCurrentRoom());

	else
		find_Enemy_In_Room(other); 
}


void Warrior::find_Storage(Storage &s, bool ammo)
{
	cout << "Searching " ;
	if (ammo)
		cout << " AMMO" << endl;
	else
		cout << " MED" << endl;

	if (currentRoom != nullptr && currentRoom->getId() != s.getRoom().getId())
		exit_Room(s.getRoom());
	else if (ammo)
		walkingPath = maze->local_A_Star(location, s.getLocation()); // enemy is in the room
	else
		walkingPath = maze->local_A_Star(location, s.getLocation());
}


void Warrior::find_Enemy_In_Room(Warrior &other)
{
	if (check_Fight(other))
	{
		if (getDistance(other) > SHOOT_MAX_DISTANCE && this->grenadeAmmo > 0)
			this->throw_Grenade(other);
		else if (getDistance(other) < SHOOT_MAX_DISTANCE)
				shoot(other);
	}
	else
		walkingPath = maze->local_A_Star(location, other.getLocation());
}


void Warrior::move_Warrior(Point2D &nextStep)
{
	maze->parts[location.GetY()][location.GetX()].reset_Type();

	location.setX(nextStep.GetX());
	location.setY(nextStep.GetY());

	maze->parts[location.GetY()][location.GetX()].setType(WARRIOR);
	update_Current_Room();
}



void Warrior::shoot(Warrior &other)
{
	srand(time(0));
	int hit = rand() % 10;

	if (hit < 3)
	{
		cout << "Warrior " << this->id << " Missed a shoot" << endl;
		return;
	}

	if (gunsAmmo <= 0)
		return;

	double distance = getDistance(other);

	cout << "Warrior " << id << " attacking" << endl;
	//check if the warrior no too far
	int damage = (SHOOT_MAX_DISTANCE - (int)distance);
	damage *= 3;
	if (damage > 0)
	{
		currentAction->updateScore();
		gunsAmmo--;
		cout << "Warrior "<< this->id << " shoot! " << other.id <<"("<<damage <<")" <<endl;
		other.injured(damage);
	}
	else
		walkingPath = maze->local_A_Star(location, other.getLocation());
}

/* Decrease the life point until dead. */
void Warrior::injured(double hitPoint)
{
	
	lifePoint = lifePoint - ((1-safetyScore/MAX_SAFTY_SCORE) * hitPoint);
	if (lifePoint <= 0)
	{
		lifePoint = 0;
		life = false;
		cout << "Warrior " << this - id << " died and lose the game!" << endl;
	}
}

void Warrior::throw_Grenade(Warrior & other)
{
	if (currentRoom == nullptr)
		return;
	//1. Variables 
	const double grenadeMaxDamage = 30;
	double damage = 0;
	double Vx, Vy; // vectors values
	Point2D targetLocation;
	calculate_Vactor_Values(Vx, Vy, other.getLocation());
	Normalizing_Vector(Vx, Vy);
	targetLocation = get_Target_By_Vector(*currentRoom, Vx, Vy);
	damage = (ConstValue::GRENADE_DEMAGE_RADIOS - getDistance(targetLocation, other.getLocation())) 
		/ ConstValue::GRENADE_DEMAGE_RADIOS * grenadeMaxDamage;
}


void Warrior::calculate_Vactor_Values(double & Vx, double & Vy, const Point2D & p)
{
	Vx = p.GetX() - this->getLocation().GetX();
	Vy = p.GetY() - this->getLocation().GetY();
}


void Warrior::Normalizing_Vector(double & Vx, double & Vy)
{
	double vectorSize = sqrt((Vx*Vx) + (Vy*Vy));
	Vx = Vx / vectorSize;
	Vy = Vy / vectorSize;
}


Point2D& Warrior::get_Target_By_Vector(Room & room, double & Vx, double & Vy)
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

void Warrior::update_Current_Room()
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