#include "Prey.h"
#include <random>
Action Prey::act(unsigned long long perception_hash)
{
    int action = random()%10;
	switch (action)
	{
	case 6:
		return UP;
	case 7:
		return RIGHT;
	case 8:
		return DOWN;
	case 9:
		return LEFT;
	default:
		return STAY;
	}
}
