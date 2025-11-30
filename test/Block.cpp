#include "Block.h"

Block::Block() {
    cells = {
        {0,0},
        {0,1},
        {1,0},
        {1,1}
    };
}

void Block::apply(Grid& g, int offsetX, int offsetY) const {
    for (const auto& cell : cells) {
        int x = offsetX + cell.first;
        int y = offsetY + cell.second;
        if (y >= 0 && y < g.size() && x >= 0 && x < g[0].size()) {
            g[y][x] = 1;
        }
    }
}
