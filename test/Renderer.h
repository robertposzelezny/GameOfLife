#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.h"


class Renderer {
public:
	Renderer(int winW, int winH, int menuW, int gridN);
	void draw(sf::RenderWindow& win, const Grid& g);
	float getCellSize() const { return cellSize; }


private:
	int winW, winH, menuW, gridN;
	float cellSize;
	sf::RectangleShape cellShape;
};