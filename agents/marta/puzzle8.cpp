#include "puzzle8.h"
#include <string>
using namespace std;

std::vector<std::pair<char, puzzle_state>> puzzle_state::get_children() const
{
	std::vector<pair<char, puzzle_state>> children;
	int zi = -1, zj = -1;
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) if (tiles[i * 3 + j] == 0) {
		zi = i;
		zj = j;
	}
	int pos = zi * 3 + zj;
	if (zi > 0) {
		int new_pos = pos - 3;
		puzzle_state new_state = *this;
		swap(new_state.tiles[pos], new_state.tiles[new_pos]);
		children.push_back({ 'D',new_state });

	}
	if (zi < 2) {
		int new_pos = pos + 3;
		puzzle_state new_state = *this;
		swap(new_state.tiles[pos], new_state.tiles[new_pos]);
		children.push_back({ 'U',new_state });

	}
	if (zj > 0) {
		int new_pos = pos - 1;
		puzzle_state new_state = *this;
		swap(new_state.tiles[pos], new_state.tiles[new_pos]);
		children.push_back({ 'R',new_state });

	}
	if (zj < 2) {
		int new_pos = pos + 1;
		puzzle_state new_state = *this;
		swap(new_state.tiles[pos], new_state.tiles[new_pos]);
		children.push_back({ 'L',new_state });

	}
	return children;
}

int puzzle_state::distance(const puzzle_state& rhs) const
{
	int distance = 0;
	for (int li = 0; li < 3; li++) {
		for (int lj = 0; lj < 3; lj++) {
			char val = tiles[li * 3 + lj];
			if (val == 0) continue;
			for (int ri = 0; ri < 3; ri++) {
				for (int rj = 0; rj < 3; rj++) {
					if (rhs.tiles[ri * 3 + rj] == val) {
						distance += abs(ri - li) + abs(rj - lj);
					}
				}
			}
		}
	}
	return distance;
}

bool puzzle_state::operator==(const puzzle_state& rhs) const
{
	for (int i = 0; i < 9; i++) {
		if (tiles[i] != rhs.tiles[i]) return false;
	}
	return true;
}

size_t puzzle_state::hash() const
{
	size_t s((size_t) tiles[0] + (size_t)tiles[1] * 9 + (size_t)tiles[2] * 81 + (size_t)tiles[3] * 729 + (size_t)tiles[4] * 6561ull + (size_t)tiles[5] * 59049ull + (size_t)tiles[6] * 531441ull +(size_t) tiles[7] * 4782969ull);
	return s;
}
