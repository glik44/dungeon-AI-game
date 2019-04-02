#pragma once
#include <queue>
#include <vector>
#include <stack>

#include "Room.h"
#include "Storage.h"
#include "Parent.h"
#include "Node.h"
#include "Definitions.h"
#include "MazeDetail.h"

using namespace std;

class Warrior;
class CompareNodes;
class CompareNodesRun;

class Maze
{
public:
	MazeDetail parts[MSIZE][MSIZE];

private:
	static Maze* maze;

	Room rooms[NUM_ROOMS];
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
	
	bool checkIfPointIsMedStorage(const Point2D &p) const;
	bool checkIfPointIsAmmoStorage(const Point2D &p) const;
	
	void drawStorage(const Storage &s);
	Room* getRooms() const;
	double getSaftyScore(Point2D &point) const;
	stack<Point2D> localAStar(Point2D &currentLocation, Point2D &targetLocation);
	
	bool AddNewNode(Node & current, Point2D & targetLocation, vector<Point2D>& gray,
		vector<Point2D>& black, vector<Parent>& parents, priority_queue<Node*, vector<Node*>, CompareNodes>& pq, int direction);
	Storage &getTargetStorage(int type, Point2D &currentLocation, Point2D &enamy);
};
