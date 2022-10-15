#pragma once
#include <vector>
#include <unordered_map>
#include "Agent.h"
#include <functional>
#include <utility>

//Below hash implementation taken from stackoverflow 
struct hash_pair final {
	template<class TFirst, class TSecond>
	size_t operator()(const std::pair<TFirst, TSecond>& p) const noexcept {
		uintmax_t hash = std::hash<TFirst>{}(p.first);
		hash <<= sizeof(uintmax_t) * 4;
		hash ^= std::hash<TSecond>{}(p.second);
		return std::hash<uintmax_t>{}(hash);
	}
};
class Map
{
public:
	struct agent_container {
		Agent* agent; // do not copy after episodes
		int x = -1;
		int y = -1;
		int intended_x = -1;
		int intended_y = -1;
		int start_x = -1;
		int start_y = -1;
		char type = 'T';
		bool updated = true;
		bool is_alive = true;
		float reward = 0;
		int hunted_count = 0;
	};
	std::unordered_map<std::pair<int,int>,char, hash_pair> entities;
	unsigned int perception_depth = 1;
	unsigned int prey_count = 0;
	unsigned int episodes = 0;
	float rd, ro, rc, rg,epsilon;
	unsigned int n;
	void add_entity(int x, int y, char type);
	char get_entity(int x, int y);
	std::vector<agent_container> agents;
	Map(int n_, int d_, float rd_, float ro_, float rc_, float rg_, float epsilon_, int episodes_);
	std::vector<char> get_perception(int x, int y);
	static unsigned long long hash_perception(const std::vector<char> perception);
	void run();
	void run_episode();
	void update();
	void display();
private:
	static inline unsigned int entity_to_int(char entity);
	inline std::pair<int, int> next_position(int x, int y, Action a);
};
