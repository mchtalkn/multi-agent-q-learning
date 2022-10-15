#include "Agent.h"
#include <limits>
#include <random>
#include <iostream>
using namespace std;
puzzle_state Agent::goal;
int Agent::repulsion_range;
std::vector<Agent*> Agent::all_agents;
std::unordered_map<puzzle_state, int> Agent::hg;
int Agent::lookahead_depth;
int Agent::get_h(const puzzle_state& sy) const
{
	if (visited_nodes.find(sy) != visited_nodes.end()) {
		return hl.at(sy);
	}
	else {
		if (hg.find(sy) != hg.end()) {
			return hg[sy];
		}
		else {
			return sy.distance(goal);
		}
	}
	return -1;
}

void Agent::update_estimation_2()
{
	
	int best_i = -1,best = numeric_limits<int>::max(), second_best_i = -1,second_best = numeric_limits<int>::max();
	vector<pair<int, puzzle_state>> W;
	unordered_set<puzzle_state> visited;
	unordered_set<puzzle_state> not_visited;
	auto children = current_state.get_children();
	for (auto& p : children) {
		not_visited.insert(p.second);
	}
	for (int depth = 0; depth < lookahead_depth; depth++) {
		unordered_set<puzzle_state> not_visited_new;
		for (const puzzle_state& s : not_visited) {
			W.push_back({ depth + get_h(s),s });
			auto grand_children = s.get_children();
			for (auto& c : grand_children) {
				if (visited.find(c.second) == visited.end()) {
					not_visited_new.insert(c.second);
				}
			}
		}
		not_visited = move(not_visited_new);
	}
	for (int i = 0; i < W.size();i++) {
		if (W[i].first < best) {
			second_best = best;
			second_best_i = best_i;
			best = W[i].first;
			best_i = i;
		}

	}
	hg[current_state] = best;
	hl[current_state] = second_best;
	
}
std::vector<std::pair<char, puzzle_state>> Agent::update_estimation()
{

	int best_i = -1, best = numeric_limits<int>::max(), second_best_i = -1, second_best = numeric_limits<int>::max();
	vector<pair<char, puzzle_state>> children = current_state.get_children();
	vector<pair<char, puzzle_state>> candidates;
	for (int i = 0; i < children.size(); i++) {
		int cost = 1 + get_h(children[i].second);
		if (cost < best) {
			candidates.clear();
			second_best = best;
			best = cost;
			candidates.push_back(children[i]);
		}
		else if (cost == best) {
			candidates.push_back(children[i]);
		}

	}
	hg[current_state] = best;
	hl[current_state] = second_best;
	return candidates;

}

int Agent::get_adjacency(const puzzle_state& s) const
{
	int adj = numeric_limits<int>::max();
	for (Agent* a : all_agents) {
		if (a == this) continue;
		adj = min(s.distance(a->current_state),adj);
		}
	return adj;
}



Agent::Agent(const puzzle_state& current_state_):current_state(current_state_),initial_state(current_state_)
{
	id = all_agents.size();
	all_agents.push_back(this);
}

Agent::Agent(const Agent& a):current_state(a.current_state),hl(a.hl),initial_state(a.initial_state)
{
	id = all_agents.size();
	all_agents.push_back(this);

}

Agent::Agent(Agent&& a):current_state(a.current_state),initial_state(a.initial_state),hl(move(a.hl)),id(a.id)
{
	int bp = 0;
}

Agent& Agent::operator=(const Agent& rhs)
{
	id = all_agents.size();
	all_agents.push_back(this);
	hl = rhs.hl;
	current_state = rhs.current_state;
	initial_state = rhs.initial_state;
	return *this;
}

void Agent::init_class(const puzzle_state& goal_, const int& repulsion_range_, const int& lookahead_depth_)
{
	hg.clear();
	all_agents.clear();
	goal = goal_;
	repulsion_range = repulsion_range_;
	lookahead_depth = lookahead_depth_;
}

void Agent::advance()
{
	if (current_state == goal) at_goal = true;
	if (at_goal) return;
	std::vector<std::pair<char, puzzle_state>> candidates = update_estimation();
	float best_adj = -1;
	int best_adj_id = -1;
	bool is_all_repulsed = true;
	float repulsion = (repulsion_range * get_h(current_state)) / (get_h(initial_state));
	vector<int> not_repulsed;
	for (int i = 0; i < candidates.size();i++) {
		const pair<int, puzzle_state>& candidate = candidates[i];
		int adj = get_adjacency(candidate.second);
		if ( adj > repulsion) {
			is_all_repulsed = false;
			not_repulsed.push_back(i);
		}
		else if (adj > best_adj) {
			best_adj = adj;
			best_adj_id = i;
		}
	}
	pair<char, puzzle_state> next_move;
	if (!is_all_repulsed) {
		int i = rand() % not_repulsed.size();
		next_move = candidates[not_repulsed[i]];
	}
	else {
		next_move = candidates[best_adj_id];
	}
	current_state = next_move.second;
	//cout << " H(current) = " << get_h(current_state) << endl;
	if (current_state == goal) at_goal = true;
	cout << "Agent" << id << " :" << next_move.first << " [ ";
	for (unsigned char c : current_state.tiles) {
		cout << (int)c << " ";
	}

	cout << "]" << endl;
}
