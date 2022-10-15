#include "Map.h"
#include "Hunter.h"
#include "Prey.h"
#include <math.h>
#include <iostream>
using namespace std;
void Map::add_entity(int x, int y, char type)
{
    entities[{x, y}] = type;
    if (type == 'h') {
        agent_container ac;
        ac.agent = new Hunter();
        ac.x = x;
        ac.y = y;
        ac.start_x = x;
        ac.start_y = y;
        ac.type = type;
        agents.push_back(ac);
    }
    else if (type == 'p') {
        agent_container ac;
        ac.agent =new Prey();
        ac.x = x;
        ac.y = y;
        ac.start_x = x;
        ac.start_y = y;
        ac.type = type;
        agents.push_back(ac);
        prey_count++;
    }
}

char Map::get_entity(int x, int y)
{
    if (x < 0 || x >= n || y < 0 || y >= n) return 'o';
    auto e = entities.find({ x,y });
    if (e == entities.end()) return '_';
    else return (*e).second;
}


Map::Map(int n_, int d_, float rd_, float ro_, float rc_, float rg_, float epsilon_, int episodes_) : n(n_), perception_depth(d_), rd(rd_), ro(ro_), rc(rc_), rg(rg_), epsilon(epsilon_), episodes(episodes_)
{
}

std::vector<char> Map::get_perception(int x, int y)
{
    std::vector<char> perception;
    for (size_t i = x-n; i <= x+n; i++)
    {
        for (size_t j = y - n; j <= y + n; j++)
        {
            perception.push_back(get_entity(i, j));
        }
    }
    return perception;
}

unsigned long long Map::hash_perception(const std::vector<char> perception)
{
    unsigned long long res = 0;
    for (int i = 0; i < perception.size();i++) {
        res += entity_to_int(perception[i])*pow(4, i);
    }
    return res;
}

void Map::run()
{
    int prey_count_initial = prey_count;
    int i = 0;
    while (i < episodes)
    {
      
        cout << "Episode:" << i << endl;
        prey_count = prey_count_initial;
        for (agent_container& ac : agents) {
            if (ac.is_alive) {
                entities.erase({ ac.x,ac.y });
            }
            ac.x = ac.start_x;
            ac.y = ac.start_y;
            entities[{ac.x, ac.y}] = ac.type;
            ac.updated = true;
            ac.is_alive = true;
            ac.hunted_count = 0;
        }
        run_episode();
        i++;
    }
}

void Map::run_episode()
{
    int step = 0;
    cout << step << endl;
    display();
    while (prey_count)
    {
        cout << step++ << endl; 
        for (agent_container& ac : agents) {
            Action action = ac.agent->act(hash_perception(get_perception(ac.x, ac.y)));
            pair<int, int> xy = next_position(ac.x, ac.y, action);
            ac.intended_x = xy.first;
            ac.intended_y = xy.second;
            ac.updated = false;
        }
        update();
        display();
    }
    
}

void Map::update()
{
    int remaining_agents = agents.size();
    int remaining_agents_check = -1;
    for (agent_container& ac : agents) {
        ac.updated = false;
        ac.reward = rd;
    }
    while (remaining_agents != remaining_agents_check) {
        remaining_agents_check = remaining_agents;
        for (int i = 0; i < agents.size(); i++) {
            agent_container& ac = agents[i];
            if (ac.updated) continue;
            if (!ac.is_alive) {
                remaining_agents--;
                ac.updated = true;
                continue;
            }
            char e = get_entity(ac.intended_x, ac.intended_y);
            if (e == 'o') {
                remaining_agents--;
                ac.updated = true;
                ac.reward = ro;
                continue;
            }
            if (ac.type == 'h') {
                bool update = true;
                for (int j = 0; j < agents.size(); j++) {
                    if (i == j) continue;
                    agent_container& ac2 = agents[j];
                    if (ac2.type == 'p') continue;
                    if (ac2.x == ac.intended_x && ac2.y == ac.intended_y) {
                        if (ac2.updated || ac2.intended_x == ac.x && ac2.intended_y == ac.y) {
                            remaining_agents--;
                            ac.updated = true;
                            ac.reward = rc;
                        }
                        update = false;
                        break;
                    }
                }
                if (update) {
                    ac.x = ac.intended_x;
                    ac.y = ac.intended_y;
                    remaining_agents--;
                    ac.updated = true;
                    ac.reward = rd;
                }
            }
        }
    }
    for (int i = 0; i < agents.size(); i++) {
        agent_container& ac = agents[i];
        if (ac.is_alive && ac.type == 'p') {
            vector<int> hunters;
            bool hunted = false;
            for (int j = 0; j < agents.size(); j++) {
                agent_container& h = agents[j];
                if (h.type != 'h') continue;
                int dist = abs(ac.x - h.x) + abs(ac.y - h.y);
                if (dist <= 1) {
                    hunters.push_back(j);
                }
                if (dist == 0) {
                    hunted = true;
                    h.hunted_count += 1;
                }
            }
            if (hunted) {
                agents[i].is_alive = false;
                prey_count--;
                entities[{ac.x, ac.y}] = 'h';
                for (int hid : hunters) {
                    agents[hid].reward = rg / hunters.size();
                }
            }
            
        }
        else continue;
    }
    vector<pair<int, int>> tobedeleted;
    for (auto p : entities) {
        if (p.second == 'h' || p.second == 'p') {
            tobedeleted.push_back(p.first);//TODO may be problematic
        }
    }
    for (auto f : tobedeleted) entities.erase(f);
    for (agent_container& ac : agents) {
        ac.agent->update(hash_perception(get_perception(ac.x, ac.y)),ac.reward);
        if (ac.is_alive) {
            entities[{ac.x, ac.y}] = ac.type;

        }
    }
}

void Map::display()
{
    vector<string> map;
    vector<int> hunt_counts;
    int pid = 1;
    int hid = 1;
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {

            string s;
            s.push_back(get_entity(x, y));
            map.push_back(s);
        }
    }
    for (auto& a : agents) {
        if (a.type == 'p' && a.is_alive) {
            map[a.x + a.y * n] = "P" + std::to_string(pid);
            pid++;
        }
        else if (a.type == 'h') {
            map[a.x + a.y * n] = "H" + std::to_string(hid);
            hid++;
            hunt_counts.push_back(a.hunted_count);
        }
    }
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            cout << map[x + y * n] << "\t";
        }
        cout << endl;
    }
    for (int i = 0; i < hunt_counts.size(); i++) {
        cout << "H" << i + 1<< "=" << hunt_counts[i] << " ";
    }
    cout << endl;
}

inline unsigned int Map::entity_to_int(char entity)
{
    switch (entity)
    {
    case '_':
        return 0;
    case 'o':
        return 1;
    case 'h':
        return 2;
    case 'p':
        return 3;
    default:
        return -1;
    }
}

inline std::pair<int, int> Map::next_position(int x, int y, Action a)
{
    switch (a) {
    case STAY:
        return { x,y };
    case UP:
        return { x,y - 1 };
    case RIGHT:
        return { x + 1,y };
    case DOWN:
        return { x,y + 1 };
    case LEFT:
        return { x - 1,y };
    default:
        return { -1,-1 };
    }
}
