#include "Grid.h"
#include <cstdlib>
#include <iostream>

Grid::Grid(int n) : N(n), cells(n, std::vector<int>(n, 0)) {}

void Grid::randomize() {
	for (int y = 0; y < N; ++y)
		for (int x = 0; x < N; ++x)
			cells[y][x] = rand() % 2;
}

void Grid::clear() {
	for (auto& r : cells) std::fill(r.begin(), r.end(), 0);
}

// Zmodyfikowana funkcja liczenia s¹siadów
int Grid::countNeighbors(int x, int y) const {
    int currentN = static_cast<int>(cells.size()); // Pobieramy faktyczny rozmiar
    int ncount = 0;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            // Logika œcian i zawijania (Torus)
            if (nx < 0) {
                if (walls && walls->left()) continue;
                nx = (nx + currentN) % currentN;
            }
            else if (nx >= currentN) {
                if (walls && walls->right()) continue;
                nx = nx % currentN;
            }

            if (ny < 0) {
                if (walls && walls->top()) continue;
                ny = (ny + currentN) % currentN;
            }
            else if (ny >= currentN) {
                if (walls && walls->bottom()) continue;
                ny = ny % currentN;
            }

            // Dodatkowe zabezpieczenie przed odczytem
            ncount += cells[ny][nx];
        }
    }
    return ncount;
}

// Zmodyfikowana funkcja kroku - u¿ywa faktycznego rozmiaru wektora
void Grid::step() {
    int currentN = static_cast<int>(cells.size());
    std::vector<std::vector<int>> next = cells;

    for (int y = 0; y < currentN; ++y) {
        for (int x = 0; x < currentN; ++x) {
            int neighbors = countNeighbors(x, y);
            if (cells[y][x] == 1)
                next[y][x] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            else
                next[y][x] = (neighbors == 3) ? 1 : 0;
        }
    }
    cells.swap(next);
}

void Grid::resize(int newN) {
    std::vector<std::vector<int>> newCells(
        newN, std::vector<int>(newN, 0)
    );

    int minN = std::min(N, newN);

    for (int y = 0; y < minN; y++) {
        for (int x = 0; x < minN; x++) {
            newCells[y][x] = cells[y][x];
        }
    }

    N = newN;
    cells.swap(newCells);
}