#pragma once
#include <vector>
#include "WallsManager.h"

class Grid {
public:
    Grid(int n);
    /**
     * @brief Randomizes all cells in the grid (alive or dead).
     */
    void randomize();
    /**
     * @brief Clears the grid by setting all cells to dead.
     */
    void clear();
    /**
     * @brief Counts the number of living neighbors around a given cell.
     * @param x Column index of the cell.
     * @param y Row index of the cell.
     * @return Number of alive neighboring cells.
     */
    int countNeighbors(int x, int y) const;
    /**
    * @brief Simulates next tick of Conway's game of life simulation.
    *
    */
    void step();

    std::vector<int>& operator[](int row) { return cells[row]; }
    const std::vector<int>& operator[](int row) const { return cells[row]; }

    int size() const { return N; }

    void setWalls(WallsManager* w) { walls = w; }
    WallsManager* getWalls() const { return walls; }
    void resize(int newN);

private:
    int N;
    std::vector<std::vector<int>> cells;
    WallsManager* walls = nullptr;
};
