#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>

typedef struct grid {
  char agentid;
  bool isCarrying;
  bool isCluster;
  bool isObstacle;
  bool isDepot;
  int goldCount;
  int crumbCount;
  float gradient;
}grid;
grid default_grid;
typedef struct sense{
  grid c,nw,n,ne,w,e,sw,s,se;
} sense;

enum actionType{
  NOACTION,
  MOVE,
  PICK_GOLD,
  PUT_GOLD,
  PASS_GOLD,
  GET_GOLD,
  ACTION_DONE
};

typedef enum direction{
  NODIRECTION,NW,N,NE,W,E,SW,S,SE
} direction;

typedef struct action{
  enum actionType type;
  enum direction dir;
  int crumbCount;
}action;
typedef struct agent{
  int x,y;
  bool isCarrying;
}agent;
grid** map;
int mapSize;
agent* agents;
int agentCount;
int totalGold;
action* actions;
int* indexes;
void advance();
action decide(agent a);
void readMap();
void printMap();
void calculateMove(int x, int y, direction dir, int* xnew, int* ynew);