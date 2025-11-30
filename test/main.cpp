#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Button.h"
#include "Glider.h"
#include "Block.h"
#include "GosperGun.h"

int GRID_N = 60;
bool running = false;

bool blockLeft = false;
bool blockRight = false;
bool blockTop = false;
bool blockBottom = false;

sf::RectangleShape wallLeft, wallRight, wallTop, wallBottom;

enum class PatternType {
    None,
    Glider,
    Block,
    GosperGun
};

PatternType selectedPattern = PatternType::None;


using Grid = std::vector<std::vector<int>>;

void randomize(Grid& g) {
    for (int y = 0; y < GRID_N; y++)
        for (int x = 0; x < GRID_N; x++)
            g[y][x] = rand() % 2;
	running = false;
}

void clearGrid(Grid& g) {
    for (auto& row : g)
        std::fill(row.begin(), row.end(), 0);
	running = false;
}

int countNeighbors(const Grid& g, int x, int y) {
    int n = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            // --- BLOKOWANIE TELEPORTACJI ---
            if (nx < 0 && blockLeft) continue;
            if (nx >= GRID_N && blockRight) continue;
            if (ny < 0 && blockTop) continue;
            if (ny >= GRID_N && blockBottom) continue;

            // --- NORMALNE WRAPOWANIE ---
            if (nx < 0) nx = GRID_N - 1;
            else if (nx >= GRID_N) nx = 0;

            if (ny < 0) ny = GRID_N - 1;
            else if (ny >= GRID_N) ny = 0;

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
    // Lewa œciana
    wallLeft.setSize({ cellSize, GRID_N * cellSize });
    wallLeft.setPosition({ 0, 0 });
	wallLeft.setFillColor(sf::Color::Transparent);

    // Prawa œciana
    wallRight.setSize({ cellSize, GRID_N * cellSize });
    wallRight.setPosition({ GRID_W - cellSize, 0 });
	wallRight.setFillColor(sf::Color::Transparent);

    // Górna
    wallTop.setSize({ GRID_N * cellSize, cellSize });
    wallTop.setPosition({ 0, 0 });
	wallTop.setFillColor(sf::Color::Transparent);

    // Dolna
    wallBottom.setSize({ GRID_N * cellSize, cellSize });
    wallBottom.setPosition({ 0, GRID_N * cellSize - cellSize });
	wallBottom.setFillColor(sf::Color::Transparent);

    int msPerGen = 150;
    sf::Clock clock;

    sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));

    sf::RectangleShape menuBg(sf::Vector2f(MENU_W, WIN_H));
    menuBg.setPosition({ GRID_W, 0 });
    menuBg.setFillColor(sf::Color(50, 50, 50));

	Button btnStart("Start/Pause (Space)", { GRID_W + 20, 50 }, { MENU_W - 40, 50 });
	Button btnRand("Randomize (R)", { GRID_W + 20, 130 }, { MENU_W - 40, 50 });
	Button btnClear("Clear (C)", { GRID_W + 20, 210 }, { MENU_W - 40, 50 });

    Button btnBlockLeft("Block Left Wall", { GRID_W + 20, 290 }, { MENU_W - 40, 50 });
    Button btnBlockRight("Block Right Wall", { GRID_W + 20, 350 }, { MENU_W - 40, 50 });
    Button btnBlockTop("Block Top Wall", { GRID_W + 20, 410 }, { MENU_W - 40, 50 });
    Button btnBlockBottom("Block Bottom Wall", { GRID_W + 20, 470 }, { MENU_W - 40, 50 });

    Button btnAddGlider("Add Glider", { GRID_W + 20, 530 }, { MENU_W - 40, 50 });
    Button btnAddBlock("Add Block", { GRID_W + 20, 590 }, { MENU_W - 40, 50 });
	Button btnAddGosperGun("Add Gosper Glider Gun", { GRID_W + 20, 650 }, { MENU_W - 40, 50 });

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

                if (me->button == sf::Mouse::Button::Left &&
                    mx >= 0 && mx < GRID_W && my >= 0 && my < GRID_N * cellSize)
                {
                    sf::Vector2i mp = sf::Mouse::getPosition(window);
                    sf::Vector2f wp = window.mapPixelToCoords(mp);

                    int gx = wp.x / cellSize;
                    int gy = wp.y / cellSize;

                    if (gx >= 0 && gx < GRID_N && gy >= 0 && gy < GRID_N)
                    {
                        if (selectedPattern == PatternType::None) {
                            grid[gy][gx] = 1 - grid[gy][gx];
                        }
                        else if (selectedPattern == PatternType::Glider) {
                            Glider gl;
                            gl.apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::Block) {
                            Block bl;
                            bl.apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::GosperGun) {
                            GosperGun gg;
                            gg.apply(grid, gy, gx);
                        }
                    }
                }

                else if (mx >= GRID_W && me->button == sf::Mouse::Button::Left) {

                    auto clicked = [&](sf::RectangleShape& b) {
                        return b.getGlobalBounds().contains(sf::Vector2f((float)mx, (float)my));
                        };

                    if (btnStart.contains(mx, my)) running = !running;
                    else if (btnRand.contains(mx, my)) randomize(grid);
                    else if (btnClear.contains(mx, my)) clearGrid(grid);
                    else if (btnBlockLeft.contains(mx, my)) {
                        blockLeft = !blockLeft;
                        btnBlockLeft.setActive(blockLeft);
                        wallLeft.setFillColor(blockLeft ? sf::Color(200, 50, 50) : sf::Color::Transparent);
                    }
                    else if (btnBlockRight.contains(mx, my)) {
                        blockRight = !blockRight;
                        btnBlockRight.setActive(blockRight);
                        wallRight.setFillColor(blockRight ? sf::Color(200, 50, 50) : sf::Color::Transparent);
                    }
                    else if (btnBlockTop.contains(mx, my)) {
                        blockTop = !blockTop;
                        btnBlockTop.setActive(blockTop);
                        wallTop.setFillColor(blockTop ? sf::Color(200, 50, 50) : sf::Color::Transparent);
                    }
                    else if (btnBlockBottom.contains(mx, my)) {
                        blockBottom = !blockBottom;
                        btnBlockBottom.setActive(blockBottom);
                        wallBottom.setFillColor(blockBottom ? sf::Color(200, 50, 50) : sf::Color::Transparent);
                    }
                    else if (btnAddGlider.contains(mx, my)) {
                        if (btnAddGlider.active()) { btnAddGlider.setActive(false);selectedPattern = PatternType::None; continue; };
                        selectedPattern = PatternType::Glider;
                        btnAddGlider.setActive(true);
                        btnAddBlock.setActive(false);
						btnAddGosperGun.setActive(false);
                    }
                    else if (btnAddBlock.contains(mx, my)) {
                        if (btnAddBlock.active()) { btnAddBlock.setActive(false);selectedPattern = PatternType::None; continue; };
                        selectedPattern = PatternType::Block;
                        btnAddBlock.setActive(true);
                        btnAddGlider.setActive(false);
						btnAddGosperGun.setActive(false);
                    }
                    else if (btnAddGosperGun.contains(mx, my)) {
                        if (btnAddGosperGun.active()) { btnAddGosperGun.setActive(false);selectedPattern = PatternType::None; continue; };
                        selectedPattern = PatternType::GosperGun;
                        btnAddGosperGun.setActive(true);
                        btnAddGlider.setActive(false);
                        btnAddBlock.setActive(false);
                    }
                }
            }
        }

        if (running && clock.getElapsedTime().asMilliseconds() > msPerGen) {
            grid = step(grid);
            clock.restart();
        }

        window.clear(sf::Color(30, 30, 30));

        window.draw(menuBg);
        window.draw(wallLeft);
        window.draw(wallRight);
        window.draw(wallTop);
        window.draw(wallBottom);

        for (int y = 0; y < GRID_N; y++) {
            for (int x = 0; x < GRID_N; x++) {
                if (grid[y][x] == 1) {
                    cell.setPosition({(float)x * cellSize, (float)y * cellSize});
                    cell.setFillColor(sf::Color::White);
                    window.draw(cell);
                }
            }
        }

		btnStart.draw(window);
		btnRand.draw(window);
		btnClear.draw(window);
        btnBlockLeft.draw(window);
        btnBlockRight.draw(window);
        btnBlockTop.draw(window);
        btnBlockBottom.draw(window);
        btnAddGlider.draw(window);
        btnAddGosperGun.draw(window);
        btnAddBlock.draw(window);
		

        window.display();
    }

    return 0;
}
