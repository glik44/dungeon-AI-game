#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_SAFTY_SCORE 9
#define NUM_OF_STORAGES 2
#define NUM_OF_WARRIORS 2
#define NUM_ROOMS 10
#define SHOOT_MAX_DISTANCE 10


#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define MAX_NUM_OF_BULLETS 50
#define MAX_NUM_OF_GRENADES 2

#define BULLET 2
#define GRENADE 3
#define LIFE 2

const int RED = 11;
const int GREEN = 12;
const int BLUE = 13;
const int YELLO = 14;
const int ORANGE = 15;
const int GREY = 16;
const int PINK = 17;
const int PURPLE = 18;
const int BLACK = 19;
const int BROWN = 20;

static const int MSIZE = 100;
static const int START = 4;
static const int NUM_OF_MEDICAL_STORAGE = 2;
static const int NUM_OF_AMMO_STORAGE = 2;

static const int SPACE = 1;
static const int WALL = 2;
static const int VISITED = 3;
static const int TARGET = 5;
static const int GRAY = 6;
static const int MEDICAL = 7;
static const int AMMO = 8;
static const int WARRIOR = 9;
static const int DOOR = 10;