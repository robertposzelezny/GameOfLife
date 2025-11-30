#include "Game.h"
#include "Grid.h"

Game::Game(int n) : g(n) {
	g.setWalls(walls);
}


Grid& Game::grid() { return g; }


void Game::updateStep() {
	g.step();
}


void Game::randomize() { g.randomize(); running = false; }


void Game::clear() { g.clear(); running = false; }