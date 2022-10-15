#pragma once
#include <vector>
class puzzle_state {
public:
	unsigned char tiles[9];
    std::vector<std::pair<char, puzzle_state>> get_children() const;
    int distance(const puzzle_state& rhs) const;
    bool operator==(const puzzle_state& rhs) const;
    size_t hash() const;
};
namespace std {
    template <> struct hash<puzzle_state>
    {
        size_t operator()(const puzzle_state& x) const
        {
            return hash<size_t>()(x.hash());
        }
    };
}
class puzzle8
{

};
