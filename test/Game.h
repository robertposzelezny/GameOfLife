#pragma once
#include "Grid.h"
#include "WallsManager.h"


class Game {
public:
	explicit Game(int n);

	Grid& grid();
	void updateStep();
	void randomize();
	void clear();
	void toggleRunning() { running = !running; }
	bool isRunning() const { return running; }
	Grid& getGrid() { return g; }
	void setWalls(WallsManager* w) { walls = w; }
	WallsManager* getWalls() const { return walls; }

private:
	Grid g;
	WallsManager* walls = nullptr;
	bool running = false;
};