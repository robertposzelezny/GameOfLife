#include "Grid.h"
#include <cstdlib>


Grid::Grid(int n) : N(n), cells(n, std::vector<int>(n, 0)) {}

extern bool blockLeft, blockRight, blockTop, blockBottom;

void Grid::randomize() {
	for (int y = 0; y < N; ++y)
		for (int x = 0; x < N; ++x)
			cells[y][x] = rand() % 2;
}


void Grid::clear() {
	for (auto& r : cells) std::fill(r.begin(), r.end(), 0);
}


int Grid::countNeighbors(int x, int y) const {
    int ncount = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;

            if (nx < 0) {
                if (walls && walls->left() && nx < 0) continue;
                nx = (nx + N) % N;
            }
            else if (nx >= N) {
                if (walls && walls->right() && nx >= N) continue;
                nx = nx % N;
            }
            if (ny < 0) {
                if (walls && walls->top() && ny < 0) continue;
                ny = (ny + N) % N;
            }
            else if (ny >= N) {
                if (walls && walls->bottom() && ny >= N) continue;
                ny = ny % N;
            }

            ncount += cells[ny][nx];
        }
    }
    return ncount;
}

void Grid::step() {
	std::vector<std::vector<int>> next = cells;
	for (int y = 0; y < N; ++y) {
		for (int x = 0; x < N; ++x) {
			int neighbors = countNeighbors(x, y);
			if (cells[y][x] == 1)
				next[y][x] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
			else
				next[y][x] = (neighbors == 3) ? 1 : 0;
		}
	}
	cells.swap(next);
}