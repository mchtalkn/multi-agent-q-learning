#include "Hunter.h"
#include <random>
float Hunter::lr, Hunter::df, Hunter::epsilon, Hunter::rd;
using namespace std;
Action Hunter::act(unsigned long long perception_hash)
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	last_state = perception_hash;
	int act;
	vector<int> best_acts;
	if (r > epsilon) {
		float best_reward = -1*INFINITY;
		for (int i = 0; i < 5; i++) {
			float reward = get_reward(last_state, i);
			if (reward >= best_reward) {
				best_acts.push_back(i);
				best_reward = reward;
			}
		}
		int i = rand() % best_acts.size();
		act = best_acts[i];
	}
	else {
		act = random()%5;
	}
	last_action = static_cast<Action>(act);

	return last_action;
}

void Hunter::update(unsigned long long state, float reward)
{
	float best_reward = -1 * INFINITY;
	int act=-1;
	for (int i = 0; i < 5; i++) {
		float reward = get_reward(state, i);
		if (reward > best_reward) {
			act = i;
			best_reward = reward;
		}
	}

	Q[last_state*5+last_action] = get_reward(last_state, last_action)+lr*(reward + df*best_reward-get_reward(last_state, last_action));
}

float Hunter::get_reward(unsigned long long state, int action)
{
	auto p = Q.find(state * 5 + action);
	if (p == Q.end()) return df;
	return (*p).second;
}
