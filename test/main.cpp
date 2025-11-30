#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Button.h"

int GRID_N = 60;
using Grid = std::vector<std::vector<int>>;

void randomize(Grid& g) {
    for (int y = 0; y < GRID_N; y++)
        for (int x = 0; x < GRID_N; x++)
            g[y][x] = rand() % 2;
}

void clearGrid(Grid& g) {
    for (auto& row : g)
        std::fill(row.begin(), row.end(), 0);
}

int countNeighbors(const Grid& g, int x, int y) {
    int n = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < GRID_N && ny >= 0 && ny < GRID_N)
                n += g[ny][nx];
        }
    }
    return n;
}

Grid step(const Grid& cur) {
    Grid next = cur;
    for (int y = 0; y < GRID_N; y++) {
        for (int x = 0; x < GRID_N; x++) {
            int neighbors = countNeighbors(cur, x, y);
            if (cur[y][x] == 1)
                next[y][x] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            else
                next[y][x] = (neighbors == 3) ? 1 : 0;
        }
    }
    return next;
}

int main() {
    srand(time(0));

    const int WIN_W = 1100;
    const int WIN_H = 800;
    const int MENU_W = 300;
    const int GRID_W = WIN_W - MENU_W;


    sf::RenderWindow window(sf::VideoMode({ WIN_W, WIN_H }), "Game of Life (Simple)");
    window.setFramerateLimit(60);

    Grid grid(GRID_N, std::vector<int>(GRID_N, 0));
    randomize(grid);

    float cellSize = std::min(GRID_W / (float)GRID_N, GRID_W / (float)GRID_N);
    bool running = false;
    int msPerGen = 150;
    sf::Clock clock;

    sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));

    sf::RectangleShape menuBg(sf::Vector2f(MENU_W, WIN_H));
    menuBg.setPosition({ GRID_W, 0 });
    menuBg.setFillColor(sf::Color(50, 50, 50));

	Button btnStart("Start/Pause (Space)", { GRID_W + 20, 50 }, { MENU_W - 40, 50 });
	Button btnRand("Randomize (R)", { GRID_W + 20, 130 }, { MENU_W - 40, 50 });
	Button btnClear("Clear (C)", { GRID_W + 20, 210 }, { MENU_W - 40, 50 });

    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) {
            if (ev->getIf<sf::Event::Closed>()) {
                window.close();
                break;
            }
            else if (auto* ke = ev->getIf<sf::Event::KeyPressed>()) {
                using K = sf::Keyboard::Key;
                switch (ke->code)
                {
                    case K::Space: running = !running; break; 
                    case K::R: randomize(grid); break; 
                    case K::C: clearGrid(grid); break; 
                    case K::S: grid = step(grid); break; 
                    case K::Up: msPerGen = std::max(10, msPerGen - 20); break; 
                    case K::Down: msPerGen += 20; break; 
                    default: break;
                }
            }
            else if (auto* me = ev->getIf<sf::Event::MouseButtonPressed>()) {
				int mx = me->position.x;
				int my = me->position.y;

                if (mx < GRID_W && me->button == sf::Mouse::Button::Left) {
                    int gx = mx / cellSize;
                    int gy = my / cellSize;
                    if (gx >= 0 && gx < GRID_N && gy >= 0 && gy < GRID_N)
                        grid[gy][gx] = 1 - grid[gy][gx];
                }
                else if (mx >= GRID_W && me->button == sf::Mouse::Button::Left) {

                    auto clicked = [&](sf::RectangleShape& b) {
                        return b.getGlobalBounds().contains(sf::Vector2f((float)mx, (float)my));
                        };

                    if (btnStart.contains(mx, my)) running = !running;
                    else if (btnRand.contains(mx, my)) randomize(grid);
                    else if (btnClear.contains(mx, my)) clearGrid(grid);
                }
            }
        }

        if (running && clock.getElapsedTime().asMilliseconds() > msPerGen) {
            grid = step(grid);
            clock.restart();
        }

        window.clear(sf::Color(30, 30, 30));

        for (int y = 0; y < GRID_N; y++) {
            for (int x = 0; x < GRID_N; x++) {
                if (grid[y][x] == 1) {
                    cell.setPosition({(float)x * cellSize, (float)y * cellSize});
                    cell.setFillColor(sf::Color::White);
                    window.draw(cell);
                }
            }
        }

        window.draw(menuBg);
		btnStart.draw(window);
		btnRand.draw(window);
		btnClear.draw(window);

        window.display();
    }

    return 0;
}
