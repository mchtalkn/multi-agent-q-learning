#include "environment.h"
#include <stdio.h> 
int main(int argc, char const *argv[]) {
  readMap();
  int i = 0;
  printf("Initial scene :\n\n");
  printMap();
  while(totalGold > 0){
    i++;
    for(int i=0;i<agentCount;i++){
      actions[i] = decide(agents[i]);
    }
    advance();
    printf("\nScene after step %d:\n\n", i);
    printMap();
    for (int i = 0; i < agentCount; i++) {
        for (int j = i + 1; j < agentCount; j++) {
            if (agents[i].x == agents[j].x && agents[i].y == agents[j].y) {
                int bp = 0;
            }
            agent a = agents[i];
            grid g = map[a.y][a.x];
            if (g.agentid != i || g.isCarrying != a.isCarrying) {
                int bp = 0;
            }
        }
    }
  }
  return 0;
}
