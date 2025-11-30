#include "Renderer.h"


Renderer::Renderer(int w, int h, int m, int n)
	: winW(w), winH(h), menuW(m), gridN(n) {
	cellSize = static_cast<float>(winW - menuW) / gridN;
	cellShape.setSize({ cellSize - 1.f, cellSize - 1.f });
}


void Renderer::draw(sf::RenderWindow& win, const Grid& g) {
	for (int y = 0; y < g.size(); ++y) {
		for (int x = 0; x < g.size(); ++x) {
			if (g[x][y] == 1) {
				cellShape.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
				cellShape.setFillColor(sf::Color::White);
				win.draw(cellShape);
			}
		}
	}
}