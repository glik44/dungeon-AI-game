#pragma once
#include <fstream>
#include "Room.h"
#include "Door.h"
#include "Storage.h"
#include "MazePart.h"
#include <queue>
#include <vector>
#include <stack>
#include <iostream>
#include "CompareActions.h"
#include "Parent.h"
#include "Node.h"

#include "Definitions.h"

using namespace std;

class Warrior;
class CompareNodes;

class Maze
{
public:
	MazePart parts[MSIZE][MSIZE];

private:
	static Maze* maze;

	Room all_rooms[NUM_ROOMS];
	Storage medicalStorage[NUM_OF_MEDICAL_STORAGE];
	Storage ammoStorage[NUM_OF_AMMO_STORAGE];

	Maze();

public:
	~Maze();
	Maze(Maze &other) = delete;
	Maze& operator=(Maze &other) = delete;
	static Maze& getInstance();
	void createStorages();
	int countSpaces(int i, int j);
	void setSaftyScores();
	void loadMazeFromFile();
	void cleanUp();

	//MazePart** getParts() const;
	void drawStorage(const Storage &s);
	Room* getRooms() const;
	double getSaftyScore(Point2D &point) const;
	bool isPointInRoom(Point2D &point) const;
	stack<Point2D> localAStar(Point2D &currentLocation, Point2D &targetLocation);
	bool AddNewNode(Node & current, Point2D & targetLocation, vector<Point2D>& gray,
		vector<Point2D>& black, vector<Parent>& parents, priority_queue<Node*, vector<Node*>, CompareNodes>& pq, int direction);
	stack<Point2D> goToTheClosestAmmoStorage(Warrior &warrior);
	stack<Point2D> goToTheClosestMedicalStorage(Warrior &warrior);
	Storage &getClosestStorage(int type, Point2D &currentLocation);
};

