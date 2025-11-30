#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Button.h"
#include "Glider.h"
#include "Block.h"
#include "GosperGun.h"
#include "UIManager.h"
#include "Pattern.h"
#include "Game.h"
#include "Grid.h"
#include "Renderer.h"
#include "WallsManager.h"

const int WIN_W = 1100;
const int WIN_H = 800;
const int MENU_W = 300;
const int GRID_W = WIN_W - MENU_W;

enum class PatternType {
    None,
    Glider,
    Block,
    GosperGun
};

PatternType selectedPattern = PatternType::None;

int main() {
    srand(time(0));

    int GRID_N = 60;

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WIN_W, WIN_H)), "Game of Life (Simple)");
    window.setFramerateLimit(60);

    Game game(GRID_N);
    game.randomize();

    float cellSize = std::min(GRID_W / (float)GRID_N, GRID_W / (float)GRID_N);

    WallsManager walls(GRID_N, cellSize, GRID_W);
    game.setWalls(&walls);

    int msPerGen = 150;
    sf::Clock clock;

    sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));

    sf::RectangleShape menuBg(sf::Vector2f(MENU_W, WIN_H));
    menuBg.setPosition(sf::Vector2f(GRID_W,0));
    menuBg.setFillColor(sf::Color(50, 50, 50));

	UIManager uiManager(GRID_W,MENU_W);
    uiManager.createButtons();

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
                    case K::Space: game.toggleRunning(); break; 
                    case K::R: game.randomize(); break; 
                    case K::C: game.clear(); break; 
                    case K::S: game.updateStep(); break; 
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
						auto& grid = game.getGrid();
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

                    if (Button* btn = uiManager.clicked(mx, my)) {
                        switch (uiManager.getCommand(btn))
                        {
						case Command::START_PAUSE:
                            game.toggleRunning();
							break;
						case Command::RANDOMIZE:
                            game.randomize();
							break;
						case Command::CLEAR:
                            game.clear();
							break;
						case Command::BLOCK_LEFT:
                            walls.toggleLeft();
                            btn->setActive(walls.left());
							break;
						case Command::BLOCK_RIGHT:
                            walls.toggleRight();
                            btn->setActive(walls.right());
							break;
						case Command::BLOCK_TOP:
                            walls.toggleTop();
                            btn->setActive(walls.top());
							break;
						case Command::BLOCK_BOTTOM:
                            walls.toggleBottom();
                            btn->setActive(walls.bottom());
							break;
						case Command::ADD_GLIDER:
							if (btn->active()) { btn->setActive(false); selectedPattern = PatternType::None; }
							else {
								selectedPattern = PatternType::Glider;
								uiManager.resetButtons();
								btn->setActive(true);
							}
							break;
						case Command::ADD_BLOCK:
							if (btn->active()) { btn->setActive(false); selectedPattern = PatternType::None; }
							else {
								selectedPattern = PatternType::Block;
								uiManager.resetButtons();
								btn->setActive(true);
							}
							break;
						case Command::ADD_GOSPER_GUN:
							if (btn->active()) { btn->setActive(false); selectedPattern = PatternType::None; }
							else {
								selectedPattern = PatternType::GosperGun;
								uiManager.resetButtons();
								btn->setActive(true);
							}
							break;
                        default:
                            break;
                        }
                    }
                }
            }
        }

        if (game.isRunning() && clock.getElapsedTime().asMilliseconds() > msPerGen) {
            game.updateStep();
            clock.restart();
        }

        window.clear(sf::Color(30, 30, 30));

		walls.draw(window);

        for (int y = 0; y < GRID_N; y++) {
            for (int x = 0; x < GRID_N; x++) {
                if (game.getGrid()[y][x] == 1) {
                    cell.setPosition({(float)x * cellSize, (float)y * cellSize});
                    cell.setFillColor(sf::Color::White);
                    window.draw(cell);
                }
            }
        }

		uiManager.draw(window);
        window.display();
    }

    return 0;
}
