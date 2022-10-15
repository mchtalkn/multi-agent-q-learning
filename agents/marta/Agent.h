#pragma once
#include "puzzle8.h"
#include <unordered_map>
#include <unordered_set>
class Agent
{
public:
	Agent(const puzzle_state& current_state);
	Agent(const Agent& a);
	Agent( Agent&& a);
	Agent& operator=(const Agent& rhs);
	static void init_class(const puzzle_state& goal_, const int& repulsion_range_, const int& lookahead_depth_);
	void advance();
	bool at_goal = false;
private:
	static puzzle_state goal;
	static int repulsion_range;
	//TODO all agents should be implemented with map and a static id counter but this version is acceptable for now
	static std::vector<Agent*> all_agents;
	static std::unordered_map<puzzle_state, int> hg;
	static int lookahead_depth;

	std::unordered_map<puzzle_state, int> hl;
	std::unordered_set<puzzle_state> visited_nodes;
	int get_h(const puzzle_state& sy) const;
	int get_adjacency(const puzzle_state& s) const;
	std::vector<std::pair<char, puzzle_state>> update_estimation();
	void update_estimation_2();
	int id;
	puzzle_state initial_state;
	puzzle_state current_state;

};
