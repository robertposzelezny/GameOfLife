#pragma once
#include <vector>
#include "WallsManager.h"

class Grid {
public:
    Grid(int n);
    void randomize();
    void clear();
    int countNeighbors(int x, int y) const;
    void step();

    std::vector<int>& operator[](int row) { return cells[row]; }
    const std::vector<int>& operator[](int row) const { return cells[row]; }

    int size() const { return N; }

    void setWalls(WallsManager* w) { walls = w; }
    WallsManager* getWalls() const { return walls; }

private:
    int N;
    std::vector<std::vector<int>> cells;
    WallsManager* walls = nullptr;
};
