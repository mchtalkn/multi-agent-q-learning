#pragma once
#include "Agent.h"
class Hunter: public Agent
{
public:
	static float lr, df,rd,epsilon;
	unsigned long long last_state;
	Action last_action;
	std::unordered_map<unsigned long long, float> Q;
	Action act(unsigned long long perception_hash) override;
	void update(unsigned long long state, float reward) override;
private:
	float get_reward(unsigned long long state, int action);
};
