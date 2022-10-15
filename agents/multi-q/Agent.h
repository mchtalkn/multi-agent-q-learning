#pragma once
#include <unordered_map>
enum Action {
	STAY,
	LEFT,
	UP,
	RIGHT,
	DOWN
};
class Agent
{
public:
	virtual Action act(unsigned long long perception_hash) = 0;
	virtual void update(unsigned long long state,float reward) {};
};
