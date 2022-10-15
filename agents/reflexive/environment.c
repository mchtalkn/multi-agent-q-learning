#include "environment.h"
#include "stdio.h"
#include "math.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h> 
grid default_grid = {
	-1,
	false,
	false,
	false,
	false,
	0,
	0,
	0
};
void advance() {
	int x, y;
	int agentNotMoved = agentCount;
	int deadlock;
	while (agentNotMoved > 0) {
		deadlock = agentNotMoved;
		for (int i = 0; i < agentCount; i++) {
			x = agents[i].x;
			y = agents[i].y;
			int xnew = x, ynew = y;
			bool flag = true;
			switch (actions[i].type)
			{
			case MOVE:
				//printf("agent %d wants to go direction %d\n", i+1, actions[i].dir);
				calculateMove(x, y, actions[i].dir, &xnew, &ynew);
				int agentatsamenode = 0;
				for (int j = 0; j < agentCount; j++) {
					int xother, yother;
					if (actions[j].type == MOVE) {
						calculateMove(agents[j].x, agents[j].y, actions[j].dir, &xother, &yother);
						if (agents[j].x == xnew && agents[j].y == ynew) {
							if (xother == x && yother == y) {
								actions[j].type = ACTION_DONE;
								actions[i].type = ACTION_DONE;
								agentNotMoved--;
								flag = false;
								break;
							}
							flag = false;
							agentNotMoved++;
							break;
						}
						if (xother == xnew && yother == ynew) agentatsamenode++;

					}
					else {
						if (agents[j].x == xnew && agents[j].y == ynew) {
							flag = false;
							actions[i].type = ACTION_DONE;
							break;
						}
					}
				}
				if (flag) {
					int agentToMove = rand() % agentatsamenode;
					if (agentatsamenode == 0) {
						int bp = 0;
					}
					for (int j = 0; j < agentCount; j++) {
						int xother, yother;
						if (actions[j].type == MOVE) {
							calculateMove(agents[j].x, agents[j].y, actions[j].dir, &xother, &yother);
							if (xother == xnew && yother == ynew) {
								if (j == i) {
									agents[i].x = xnew;
									agents[i].y = ynew;
									if (map[y][x].agentid == i) {
										map[y][x].agentid = -1;
										map[y][x].isCarrying = false;

									}
									map[agents[i].y][agents[i].x].agentid = i;
									map[agents[i].y][agents[i].x].isCarrying = agents[i].isCarrying;
									map[y][x].crumbCount += actions[i].crumbCount;
								}
								agentToMove--;
								actions[i].type = ACTION_DONE;
							}

						}
					}
				}
				break;
			case PICK_GOLD:
				agents[i].isCarrying = true;
				map[y][x].isCarrying = true;
				map[agents[i].y][agents[i].x].goldCount--;
				map[y][x].crumbCount += actions[i].crumbCount;
				actions[i].type = ACTION_DONE;
				break;
			case PUT_GOLD:
				agents[i].isCarrying = false;
				map[y][x].isCarrying = false;
				map[agents[i].y][agents[i].x].goldCount++;
				totalGold--;
				map[y][x].crumbCount += actions[i].crumbCount;
				actions[i].type = ACTION_DONE;
				break;
			case PASS_GOLD:
				calculateMove(x, y, actions[i].dir, &xnew, &ynew);
				int xcheck, ycheck;
				int gettingAgentId = map[ynew][xnew].agentid;
				if (gettingAgentId != -1 && actions[gettingAgentId].type == GET_GOLD) {
					calculateMove(xnew, ynew, actions[gettingAgentId].dir, &xcheck, &ycheck);
					if (xcheck == x && ycheck == y) {
						actions[gettingAgentId].type = ACTION_DONE;
						actions[i].type = ACTION_DONE;
						agents[i].isCarrying = false;
						agents[gettingAgentId].isCarrying = true;
						map[y][x].isCarrying = false;
						map[ynew][xnew].isCarrying = true;
						agentNotMoved--;
					}
				}
				break;
			case GET_GOLD:
				break;
			case NOACTION:
				actions[i].crumbCount = 0;
				break;
			case ACTION_DONE:
				agentNotMoved++;
			default:
				break;
			}
			agentNotMoved--;
		}
		if (deadlock == agentNotMoved) {
			int bp = 0;
			agent* deadlocked = malloc(sizeof(agent) * agentCount);
			for (int i = 0; i < agentCount; i++) {
				deadlocked[i] = agents[i];
				x = agents[i].x;
				y = agents[i].y;
				int xnew = x, ynew = y;
				if (actions[i].type == MOVE) {
					calculateMove(deadlocked[i].x, deadlocked[i].y, actions[i].dir, &deadlocked[i].x, &deadlocked[i].y);
				}
			}
			for (int i = 0; i < agentCount; i++) {
				for (int j = i + 1; j < agentCount; j++) {
					if (deadlocked[i].x == deadlocked[j].x && deadlocked[i].y == deadlocked[j].y) {
						int bp = 0;
						free(deadlocked);
						return;
					}
				}
			}
			for (int i = 0; i < agentCount; i++) {
					if (actions[i].type == MOVE) {
						int xnew, ynew;
						calculateMove(agents[i].x, agents[i].y, actions[i].dir, &xnew, &ynew);
						agents[i].x = xnew;
						agents[i].y = ynew;
						if (map[y][x].agentid == i) {
							map[y][x].agentid = -1;
							map[y][x].isCarrying = false;

						}
						map[agents[i].y][agents[i].x].agentid = i;
						map[agents[i].y][agents[i].x].isCarrying = agents[i].isCarrying;
						map[y][x].crumbCount += actions[i].crumbCount;
						actions[i].type = ACTION_DONE;
				}
			}
			free(deadlocked);
			return;
		}
	}
}

sense getSense(int x, int y) {
	sense s;
	s.nw = map[y-1][x-1];
	s.n = map[y-1][x];
	s.ne = map[y-1][x+1];
	s.w = map[y][x-1];
	s.c = map[y][x];
	s.e = map[y][x+1];
	s.sw = map[y+1][x-1];
	s.s = map[y+1][x];
	s.se = map[y+1][x+1];
	return s;
}
action decide(agent a){
  action act;
  act.type = NOACTION;
  act.dir = NODIRECTION;
  act.crumbCount = 0;
  sense sensed = getSense(a.x, a.y);
  grid* g = (void*)&sensed;
  bool isAllObstacle = false;
  int nonObsGrid = 0;
  for (int i = 1; i < 9; i++) {
	  if (!g[i].isObstacle) nonObsGrid++;
  }
  if (nonObsGrid == 0) isAllObstacle = true;
  if (a.isCarrying ) {
	  if (sensed.c.isDepot) {
		  act.type = PUT_GOLD;
		  return act;

	  }
	  else {
		  if (isAllObstacle) {
			  act.type = NOACTION;
			  return act;
		  }
		  if (sensed.c.crumbCount > 50) {
			  int p= 5000.0 / (sensed.c.crumbCount);
			  int rnd = rand();
			  if (rnd % 100 > p ) {
				  int dirind = (rand() % nonObsGrid);
				  int dir = 1;
				  while (dirind || g[dir].isObstacle) {
					  if (!g[dir].isObstacle) dirind--;
					  dir++;
				  }
				  act.dir = dir;
				  act.type = MOVE;
				  if (g[dir].agentid != -1 && g[dir].isCarrying == false) {
					  act.type = PASS_GOLD;
				  }
				  return act;
			  }
		  }
		  float mingrad = 0;
		  for (int i = 1; i < 9; i++) {
			  if (g[i].gradient > mingrad && !g[i].isObstacle) {
				  mingrad = g[i].gradient;
				  act.dir = i;
			  }
		  }
		  act.crumbCount = 2;
		  if (g[act.dir].agentid != -1) {
			  if (g[act.dir].isCarrying == false) {
				  act.type = PASS_GOLD;
			  }
			  else {

			  }
		  }
		  else {
			  act.type = MOVE;
		  }
		  return act;
	  }

  }
  else if (!a.isCarrying) {
	  if (sensed.c.isCluster && sensed.c.goldCount > 0) {
		  act.type = PICK_GOLD;
		  return act;
	  }
	  else if (isAllObstacle) {
		  act.type = NOACTION;
		  return act;
	  }
	  else {
		  if (sensed.c.crumbCount > 50) {
			  act.crumbCount = -1;
			  int p = 5000.0 / (sensed.c.crumbCount);
			  int rnd = rand();
			  if (rnd % 100 > p) {
				  int dirind = (rand() % nonObsGrid);
				  int dir = 1;
				  while (dirind || g[dir].isObstacle) {
					  if (!g[dir].isObstacle) dirind--;
				  }
				  act.dir = dir;
				  act.type = MOVE;
				  if (g[dir].agentid != -1 && g[dir].isCarrying == false) {
					  act.type = GET_GOLD;
				  }

				  return act;
			  }
		  }
		  for (int i = 1; i < 9; i++) {
			  if (g[i].agentid != -1 && g[i].isCarrying == true && g[i].gradient < g[0].gradient) {
				  act.dir = i;
				  act.type = GET_GOLD;
				  return act;
			  }
		  }
		  if (sensed.c.crumbCount > 0) {
			  act.crumbCount = -1;
			  float maxgrad = sensed.c.gradient;
			  float crumbCount = sensed.c.crumbCount;
			  for (int i = 1; i < 9; i++) {
				  if (g[i].gradient < maxgrad && !g[i].isObstacle && g[i].crumbCount > 0) {
					  maxgrad = g[i].gradient;
					  act.dir = i;
				  }
			  }
			  if (act.dir != NODIRECTION) {
				  act.crumbCount = -1;
				  act.type = MOVE;
				  return act;

			  }
			  else {
				  int dirind = (rand() % nonObsGrid);
				  int dir = 1;
				  while (dirind || g[dir].isObstacle) {
					  if (!g[dir].isObstacle) dirind--;
					  dir++;
				  }
				  act.dir = dir;
				  act.type = MOVE;
				  act.crumbCount = -1;
				  if (g[act.dir].agentid != 0 && g[act.dir].isCarrying) {
					  act.type = GET_GOLD;
				  }
				  return act;
			  } //TODO ilerleyece�in yerde agent varsa veya hi�bir yere ilerleyemediyse ne yaps�n

		  }
		  else { //TODO check whether there exists somewhere to move or not
			  grid* g = (void*)&sensed;
			  int dirind = (rand() % nonObsGrid);
			  int dir = 1;
			  while (dirind || g[dir].isObstacle) {
				  if (!g[dir].isObstacle) dirind--;
				  dir++;
			  }
			  act.dir = dir;
			  act.type = MOVE;
			  return act;
		  }
	  }

  }
  act.dir = NODIRECTION;
  act.type = NOACTION;
  act.crumbCount = 0;
  return act;
}
void readMap(){
	FILE* ptr = fopen("env.inp", "r");
	if (ptr == NULL) {
		printf("no such file.");
		exit(-1);
	}
	srand(1);
	fscanf(ptr,"%d", &mapSize);
	map = malloc(sizeof(grid*) * (mapSize+2) );
	for (int i = 0; i < mapSize+2; i++) {
		map[i] = malloc(sizeof(grid) * (mapSize + 2));
		for (int j = 0; j < mapSize + 2; j++) {
			map[i][j] = default_grid;
			if (i == 0 || i == mapSize + 1) {
				map[i][j].isObstacle = true;
			}
		}
		map[i][0].isObstacle = true;
		map[i][mapSize + 1].isObstacle = true;
	}
	fscanf(ptr, "%d", &agentCount);
	int clusterCount;
	fscanf(ptr, "%d", &clusterCount);
	int numbobs;
	fscanf(ptr, "%d", &numbobs);
	int x, y;
	int i = agentCount;
	char c;
	agents = malloc(sizeof(agent) * agentCount);
	actions = malloc(sizeof(action) * agentCount);
	while (i--)
	{
		fscanf(ptr, "\n%c %d %d",&c, &x, &y);
		agents[i].x = x;
		agents[i].y = y;
		agents[i].isCarrying = false;
		map[y][x].agentid = i;

	}
	i = clusterCount;
	while (i--)
	{
		int g = 0;
		fscanf(ptr, "\n%c %d %d %d",&c, &x, &y, &g);
		map[y][x].isCluster = true;
		map[y][x].goldCount = g;
		totalGold += g;
	}
	while (numbobs--)
	{
		fscanf(ptr, "\n\%c %d %d",&c,&x,&y);
		map[y][x].isObstacle = true;
	}
	fscanf(ptr, "\n%c %d %d",&c, &x, &y);
	map[y][x].isDepot = true;
	for (int i = 0; i < mapSize + 2; i++) {
		for (int j = 0; j < mapSize + 2; j++){
			map[i][j].gradient = mapSize*mapSize/sqrtf((i - y) * (i - y) + (j - x) * (j - x));
		}
	}
	indexes = malloc(sizeof(int) * agentCount);
	for (int i = 0; i < agentCount; i++) indexes[i] = i;
}
void printMap() {
	for (int y = 0; y < mapSize + 2; y++) {
		for (int x = 0; x < mapSize + 2; x++) {
			grid g = map[y][x];
			if (g.isObstacle) {
				printf("O ");
			}
			else if (g.agentid > -1) {
				if (g.isCarrying) {
					printf("R%d", g.agentid+1);
				}
				else {
					printf("r%d", g.agentid+1);
				}
			}
			else if (g.isCluster) {
				printf("G%d", g.goldCount);
			}
			else if (g.isDepot) {
				printf("D%d", g.goldCount);
			}
			else {
				if (g.crumbCount > 0) {
					printf("*%d", g.crumbCount);
				}
				else if (g.crumbCount != 0) {
					int bp = 0;
				}else
				printf("__");
			}
			printf("\t");
		}
		printf("\n");
	}
}
void calculateMove(int x, int y, direction dir, int* xnew, int* ynew)
{
	switch (dir)
	{
	case NODIRECTION:
		*xnew = x;
		*ynew = y;
		break;
	case NW:
		*xnew = x - 1;
		*ynew = y - 1;
		break;
	case N:
		*xnew = x;
		*ynew = y - 1;
		break;
	case NE:
		*xnew = x + 1;
		*ynew = y - 1;
		break;
	case W:
		*xnew = x - 1;
		*ynew = y;
		break;
	case E:
		*xnew = x + 1;
		*ynew = y;
		break;
	case SW:
		*xnew = x - 1;
		*ynew = y + 1;
		break;
	case S:
		*xnew = x;
		*ynew = y + 1;
		break;
	case SE:
		*xnew = x + 1;
		*ynew = y + 1;
		break;
	default:
		break;
	}
}
