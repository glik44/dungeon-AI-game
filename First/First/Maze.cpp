#include "Maze.h"
#include "CompareNodes.h"
#include "CompareStorages.h"
#include "Warrior.h"
#include "Door.h"

#include <iostream>
#include <fstream>

Maze* Maze::maze = nullptr;

Maze::Maze()
{
	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
			parts[i][j] = *new MazeDetail(new Point2D(j, i));
	}
	loadMazeFromFile();
	setSaftyScores();
	createStorages();
}

Maze::~Maze()
{
	delete maze;
	// ..
}

Maze& Maze::getInstance()
{
	if (maze == nullptr)
		maze = new Maze();
	return *maze;
}

void Maze::loadMazeFromFile()
{
	ifstream file;

	// 1. load maze stracture
	file.open("maze.txt");
	int col, row, type;
	file >> col;
	file >> row;
	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
		{
			file >> type;
			parts[i][j].setType(type);
			parts[i][j].setOriginType(type);
		}
	}
	file.close();

	// 2. load rooms 
	file.open("rooms.txt");
	int size, x, y, h, w;
	file >> size;
	for (int i = 0; i < NUM_ROOMS; i++)
	{
		file >> x;
		file >> y;
		file >> h;
		file >> w;
		int idx = i + 1;
		rooms[i] = *new Room(idx, *new Point2D(x, y), w, h);
	}
	file.close();

	// 3. load doors
	file.open("doors2.txt");
	int from, x1, y1, x2, y2, rooms_num;
	file >> size;

	// sourceRoom ,enter and exit location of the doors, vactor of destionations
	for (int i = 0; i < size; i++)
	{
		file >> from;
		file >> x1;
		file >> y1;
		file >> x2;
		file >> y2;
		file >> rooms_num;
		Door* door = new Door(rooms[from - 1], *new Point2D(x1, y1), *new Point2D(x2, y2));

		for (int j = 0; j < rooms_num; j++) {
			int roomIndex;
			file >> roomIndex;
			door->addDestination(rooms[roomIndex - 1]);
		}
		rooms[from - 1].addDoor(*door);
	}
	file.close();
}

bool Maze::checkIfPointIsMedStorage(const Point2D & p) const
{
	return (medicalStorage[0].getLocation() == p) || (medicalStorage[1].getLocation() == p);
}

bool Maze::checkIfPointIsAmmoStorage(const Point2D & p) const
{
	return (ammoStorage[0].getLocation() == p) || (ammoStorage[1].getLocation() == p);
}

void Maze::createStorages() {
	//Create ammo storgae
	for (int i = 0; i < NUM_OF_AMMO_STORAGE; i++)
	{
		int roomIndex = i;
		// get random point in the selected room
		Point2D &rand = rooms[roomIndex].getRandomPointInRoom();
		Storage *s = new Storage(rooms[roomIndex], rand, AMMO);
		parts[rand.GetY()][rand.GetX()].setOriginType(AMMO);
		parts[rand.GetY()][rand.GetX()].setType(AMMO);
		ammoStorage[i] = *s;
		drawStorage(*s);
	}

	//Create medical storgae
	for (int i = 0; i < NUM_OF_MEDICAL_STORAGE; i++)
	{
		int roomIndex = NUM_ROOMS - i - 1;
		Point2D &rand = rooms[roomIndex].getRandomPointInRoom();
		Storage *s = new Storage(rooms[roomIndex],rand, MEDICAL);
		parts[rand.GetY()][rand.GetX()].setOriginType(MEDICAL);
		parts[rand.GetY()][rand.GetX()].setOriginType(MEDICAL);
		medicalStorage[i] = *s;
		drawStorage(*s);
	}
}

void Maze::drawStorage(const Storage &s)
{
	Point2D &location = s.getLocation();
	parts[location.GetY()][location.GetX()].setType(s.getType());
}

void Maze::setSaftyScores()
{
	int spaces = 0;
	int total = MAX_SAFTY_SCORE; 

	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
		{
			spaces = countSpaces(i, j); // check 3 X 3 square

			// max spaces num = 8 -> not safe -> safty score is low
			// min spaces num = 1 -> very safe -> safty score is high
			int score = 1 - (spaces / total);
			parts[i][j].setSaftyScore(score);
		}
	}
} 

int Maze::countSpaces(int i, int j)
{
	int count = 0;
	for (int k = i - 1; k < i + 1; k++)
	{
		for (int l = j - 1; l < j + 1; l++)
		{
			if (parts[k][l].getType() == SPACE)
				count++;
		}
	}
	return count;
}

Room* Maze::getRooms() const
{
	return (Room*)rooms;
}

double Maze::getSaftyScore(Point2D &point) const
{
	return parts[point.GetY()][point.GetX()].getSaftyScore();
}



Storage &Maze::getTargetStorage(int type, Point2D &currentLocation, Point2D &enamy)
{
	priority_queue<StorageNode*, vector<StorageNode*>, CompareStorages> targetQueue;

	if (type == Action::FIND_AMMO)
	{
		for (int i = 0; i < NUM_OF_AMMO_STORAGE; i++)
		{
			StorageNode *n = new StorageNode(currentLocation, enamy, ammoStorage[i]);
			targetQueue.push(n);
		}		
	}

	else
	{
		for (int i = 0; i < NUM_OF_MEDICAL_STORAGE; i++)
			targetQueue.push(new StorageNode(currentLocation, enamy, medicalStorage[i]));
		
	}
	
	return targetQueue.top()->getStorage();
}

/*
this function assumed that the targetLocation in the same room,
and the targetLoction != location.
the function using a* algorithm to reach the targetLocation
*/
stack<Point2D> Maze::localAStar(Point2D &currentLocation, Point2D &targetLocation)
{
	//Variables for A* algorithm
	stack<Point2D> walkingPath;
	Node *current = nullptr;
	priority_queue<Node*, vector<Node*>, CompareNodes> pq; // the compare node class may not considare the saftyScore.
	vector<Point2D>::iterator gray_it;
	vector<Point2D*>::iterator black_it;
	vector <Point2D> gray;
	vector <Point2D> black;
	vector <Parent> parents;
	bool finished = false;

	//A* action
	pq.emplace(new Node(currentLocation, targetLocation, 0));
	while (!finished)
	{
		if (pq.empty())
		{
			cout << "pq should not be empty in this function!\n fnuctin:lookForEnemyInRoom ";
			return walkingPath;
		}

		vector<Parent>::iterator itr;

		//delete allocation
		if (current != nullptr)
			delete(current);

		current = pq.top();
		pq.pop(); // remove it from pq
		parents.push_back(*new Parent(current->GetPoint(), current->GetPoint(), false));

		// the target has been found
		if (current->GetH() == 0)
		{
			finished = true;

			// go back to start and enter the steps to walkingPath 
			itr = find(parents.begin(), parents.end(), Parent(current->GetPoint(), current->GetPoint(), true));
			walkingPath.push((itr->GetCurrent()));

			while (itr->HasParent())
			{
				Point2D &tmp_prev = itr->GetPrev();
				Point2D &tmp_cur = itr->GetCurrent();
				walkingPath.push(tmp_cur);
				itr = find(parents.begin(), parents.end(),
					Parent(tmp_prev, current->GetPoint(), true));
			}
		}

		// check the neighbours
		else
		{
			//remove current from gray 
			gray_it = find(gray.begin(), gray.end(), current->GetPoint());
			if (gray_it != gray.end())
				gray.erase(gray_it);

			// and paint it black
			black.push_back(current->GetPoint());

			// try to go UP
			AddNewNode(*current, targetLocation, gray, black, parents, pq, ConstValue::UP);
			// try to go DOWN
			AddNewNode(*current, targetLocation, gray, black, parents, pq, ConstValue::DOWN);
			// try to go LEFT
			AddNewNode(*current, targetLocation, gray, black, parents, pq, ConstValue::LEFT);
			// try to go RIGHT
			AddNewNode(*current, targetLocation, gray, black, parents, pq, ConstValue::RIGHT);
		}
	}
	return walkingPath;
}


bool Maze::AddNewNode(Node & current, Point2D & targetLocation, vector<Point2D>& gray, vector<Point2D>& black, vector<Parent>& parents, priority_queue<Node*, vector<Node*>, CompareNodes>& pq, int direction)
{
	Node* tmp;
	Point2D* pt;
	vector<Point2D>::iterator gray_it;
	vector<Point2D>::iterator black_it;
	double space_weight = 0.1, wall_weight = 5, weight;
	int dx, dy;
	bool finished = false;

	switch (direction)
	{
	case ConstValue::UP:
		dx = 0;
		dy = -1;
		break;
	case ConstValue::DOWN:
		dx = 0;
		dy = 1;
		break;
	case ConstValue::LEFT:
		dx = -1;
		dy = 0;
		break;
	case ConstValue::RIGHT:
		dx = 1;
		dy = 0;
		break;
	} // switch

	if (maze->parts[current.GetPoint().GetY()][current.GetPoint().GetX() - 1].getType() == TARGET)
		finished = true;

	if (direction == ConstValue::UP && current.GetPoint().GetY() > 0 ||
		direction == ConstValue::DOWN && current.GetPoint().GetY() < MSIZE - 1 ||
		direction == ConstValue::LEFT && current.GetPoint().GetX() > 0 ||
		direction == ConstValue::RIGHT && current.GetPoint().GetX() < MSIZE - 1)
	{
		pt = new Point2D(current.GetPoint().GetX() + dx, current.GetPoint().GetY() + dy);
		gray_it = find(gray.begin(), gray.end(), *pt);
		black_it = find(black.begin(), black.end(), *pt);
		if (gray_it == gray.end() && black_it == black.end()) // this is a new point
		{
			// very important to tunnels
			if (maze->parts[current.GetPoint().GetY() + dy][current.GetPoint().GetX() + dx].getType() == WALL)
				weight = wall_weight;
			else weight = space_weight;
			// weight depends on previous weight and wheater we had to dig
			// to this point or not
			tmp = new Node(*pt, targetLocation, current.GetG() + weight);
			pq.emplace(tmp); // insert first node to priority queue
			gray.push_back(*pt); // paint it gray
			// add Parent
			parents.push_back(Parent(tmp->GetPoint(), current.GetPoint(), true));
		}
	}
	return finished;
}