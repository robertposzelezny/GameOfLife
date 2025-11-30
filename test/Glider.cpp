#include "Glider.h"
#include <vector>

Glider::Glider() {
	cells = {
		{0,1},
		{1,2},
		{2,0},
		{2,1},
		{2,2}
	};
}

void Glider::apply(Grid& g, int offsetX, int offsetY) const {
	for (const auto& cell : cells) {
		int x = offsetX + cell.first;
		int y = offsetY + cell.second;
		if (x >= 0 && x < g.size() && y >= 0 && g[0].size()) {
			g[x][y] = 1;
		}
	}
}