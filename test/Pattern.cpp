#include "Pattern.h"

void Pattern::apply(Grid& g, int offsetY, int offsetX) const{
    for (const auto& cell : cells) {
        int x = offsetX + cell.first;
        int y = offsetY + cell.second;
        if (y >= 0 && y < (int)g.size()) {
            if (x >= 0 && x < (int)g[y].size()) {
                g[y][x] = 1;
            }
        }
    }
}