// main.cpp : Defines the entry point for the application.
//

#include "ceng580hw2.h"
#include "Agent.h"
#include <iostream>
using namespace std;

int main()
{
	puzzle_state g = {
		1,2,3,
		7,6,5,
		8,0,4

	};
	puzzle_state s = {
		8,1,3,
		7,2,4,
		6,5,0
	};
	int num_agents;
	cin >> num_agents;
	for (int i = 0; i < 9; i++) {
		int a;
		cin >> a;
		s.tiles[i] = a;
	}
	for (int i = 0; i < 9; i++) {
		int a;
		cin >> a;
		g.tiles[i] = a;
	}
	Agent::init_class(g, 2, 1);
	vector<Agent*> agents;
	for (int i = 0; i < num_agents; i++) agents.push_back(new Agent(s));
	bool goal_found = false;
	int step = 0;
	while (!goal_found) {
		cout << "Step :" << step++ << endl;
		for (Agent* agent : agents) {
			agent->advance();
			if (agent->at_goal) {
				goal_found = true;
			}
		}
	}
	return 0;
}
