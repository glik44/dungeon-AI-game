#include "GLUT.h"
#include "Point2D.h"
#include "Room.h"
#include "Warrior.h" 
#include "Maze.h"
#include "Definitions.h"

using namespace std;

//temp
int counter = 0;

const int W = 600; // window width
const int H = 600; // window height

const double SQSIZE = 2.0 / MSIZE;

Warrior *warriors[NUM_OF_WARRIORS];

Point2D start,target;

Maze* maze;

void drawWarrior(const Warrior &warrior);
void createWarriors();

void delay(int number_of_seconds)
{
	// Converting time into milli_seconds 
	int milli_seconds = 10 * number_of_seconds;

	// Stroing start time 
	clock_t start_time = clock();

	// looping till required time is not acheived 
	while (clock() < start_time + milli_seconds);

}

void init()
{
	maze = &Maze::getInstance();
	createWarriors();

	glClearColor(0.7, 0.7, 0.7, 0);
	glOrtho(-1, 1, -1, 1, -1, 1);
}

void createWarriors()
{
	srand(time(0));

	for (int i = 0; i < NUM_OF_WARRIORS; i++)
	{
		Room& r = maze->getRooms()[rand() % NUM_ROOMS];
		int y = r.GetCenter().GetY();
		int x = r.GetCenter().GetX();
		maze->parts[y][x].setType(WARRIOR);
		Warrior *w = new Warrior(r, *new Point2D(x, y), ((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)));
		warriors[i] = w;
		drawWarrior(*warriors[i]);
	}
}

void drawWarrior(const Warrior &warrior)
{
	Point2D location = warrior.getLocation();
	maze->parts[location.GetY()][location.GetX()].setType(WARRIOR);
}

void DrawMaze()
{
	int i, j;

	for(i = 0;i<MSIZE;i++)
		for (j = 0; j < MSIZE; j++)
		{
			switch (maze->parts[i][j].getType())
			{
			case WALL:
				glColor3d(0.4, 0, 0); // dark red;
				break;
			case SPACE:
				glColor3d(1, 1, 1); // white;
				break;
			case VISITED:
				glColor3d(0, 0.9, 0); // green;
				break;
			case TARGET:
				glColor3d(1,0,0 ); // RED;
				break;
			case GRAY:
				glColor3d(1, .8, 0); // ORANGE;
				break;
			case MEDICAL:
				glColor3d(0,0,1); //blue
				break;
			case AMMO:
				glColor3d(1,0,0); //red
				break;
			case WARRIOR:
				glColor3d(0, 0, 0); // BLACK
				break;

			}
			// draw square
			glBegin(GL_POLYGON);
				glVertex2d(j*SQSIZE - 1- SQSIZE/2, i*SQSIZE - 1+SQSIZE/2);
				glVertex2d(j*SQSIZE - 1 + SQSIZE / 2, i*SQSIZE - 1 + SQSIZE / 2);
				glVertex2d(j*SQSIZE - 1 + SQSIZE / 2, i*SQSIZE - 1 - SQSIZE / 2);
				glVertex2d(j*SQSIZE - 1 - SQSIZE / 2, i*SQSIZE - 1 - SQSIZE / 2);
			glEnd();
		}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	DrawMaze();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void idle()
{
	if (!warriors[0]->isAlive() || !warriors[1]->isAlive())
		return;

	// temp
	(warriors[0])->selectMission(*warriors[1]);
	(warriors[1])->selectMission(*warriors[0]);

	glutPostRedisplay(); // calls indirectly to display
	delay(5);
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens
	init();

	glutMainLoop();
}


