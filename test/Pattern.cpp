#include "Pattern.h"

void Pattern::apply(Grid& g, int offsetX, int offsetY) const{
	for (const auto& cell : cells) {
		int x = offsetX + cell.first;
		int y = offsetY + cell.second;
		if (y >= 0 && y < g.size() && x >= 0 && x < g[0].size()) {
			g[x][y] = 1;
		}
	}
}