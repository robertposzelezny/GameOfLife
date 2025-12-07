#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Button.h"
#include "InputField.h"

#include "Game.h"
#include "Grid.h"
#include "UIManager.h"
#include "WallsManager.h"

#include "Pattern.h"
#include "Glider.h"
#include "Block.h"
#include "GosperGun.h"
#include "SimkinGliderGun.h"
#include "LWSS.h"
#include "Pulsar.h"
#include "Pentadecathlon.h"
#include "Acorn.h"
#include "CanadaGoose.h"
#include "Spiral.h"
#include "RPentomino.h"

Game::Game(int n) : g(n) {}

Grid& Game::grid() { return g; }
void Game::updateStep() { g.step(); }
void Game::randomize() { g.randomize(); running = false; }
void Game::clear() { g.clear(); running = false; }

void Game::runGame() {

    srand(time(0));

    int GRID_N = 200;

    auto mode = sf::VideoMode::getFullscreenModes()[0];

    WIN_W = mode.size.x;
    WIN_H = mode.size.y;

    MENU_W = WIN_W * 0.20f;
    GRID_W = WIN_W - MENU_W;

    sf::RenderWindow window(mode, "Game of Life", sf::Style::Close);
    window.setFramerateLimit(60);

    Game game(GRID_N);
    game.randomize();

    game.cellSize = GRID_W / static_cast<float>(GRID_N);

    WallsManager walls(GRID_N, game.cellSize, GRID_W);
    game.setWalls(&walls);

    int msPerGen = 150;
    sf::Clock clock;

    sf::RectangleShape cell(sf::Vector2f(game.cellSize - 1, game.cellSize - 1));

    sf::RectangleShape menuBg(sf::Vector2f(MENU_W, WIN_H));
    menuBg.setPosition(sf::Vector2f(GRID_W, 0));
    menuBg.setFillColor(sf::Color(50, 50, 50));

    UIManager uiManager(GRID_W, MENU_W);
    uiManager.createButtons();

    InputField gridSizeInput(sf::Vector2f(GRID_W + 20, 820), sf::Vector2f(MENU_W - 40, 50));

    auto findPatternIndex = [&](PatternType t) {
        for (size_t i = 0; i < patternOrder.size(); i++)
            if (patternOrder[i] == t) return static_cast<int>(i);
        return -1;
        };

    while (window.isOpen()) {

        while (auto ev = window.pollEvent()) {

            if (ev->getIf<sf::Event::TextEntered>()) {
                gridSizeInput.handleEvent(*ev, window);
            }

            if (ev->getIf<sf::Event::MouseButtonPressed>()) {
                gridSizeInput.handleEvent(*ev, window);
            }

            if (auto* ke2 = ev->getIf<sf::Event::KeyPressed>()) {
                if (ke2->code == sf::Keyboard::Key::Enter) {

                    int newSize = gridSizeInput.getValue();

                    if (newSize >= 5 && newSize <= 500) {
                        GRID_N = newSize;

                        game.grid().resize(GRID_N);
                        game.cellSize = GRID_W / (float)GRID_N;

                        walls = WallsManager(GRID_N, game.cellSize, GRID_W);
                        game.setWalls(&walls);
						cell.setSize(sf::Vector2f(game.cellSize - 1, game.cellSize - 1));
                    }
                }
            }

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
                case K::Left:
                case K::Right:

                    if (selectedPattern != PatternType::None)
                    {
                        int idx = findPatternIndex(selectedPattern);

                        if (idx != -1)
                        {
                            if (ke->code == K::Right) idx++;
                            else idx--;

                            if (idx < 0) idx = patternOrder.size() - 1;
                            if (idx >= (int)patternOrder.size()) idx = 0;

                            selectedPattern = patternOrder[idx];
                            lastPattern = selectedPattern;
                            Button* pbtn = uiManager.getPatternButton();

                            if (pbtn)
                            {
                                if (selectedPattern == PatternType::Block)
                                    pbtn->setLabel("Add Pattern (Block)");
                                else if (selectedPattern == PatternType::Glider)
                                    pbtn->setLabel("Add Pattern (Glider)");
                                else if (selectedPattern == PatternType::GosperGun)
                                    pbtn->setLabel("Add Pattern (Gosper Gun)");
								else if (selectedPattern == PatternType::SimkinGliderGun)
									pbtn->setLabel("Add Pattern (SGG)");
                                else if (selectedPattern == PatternType::LWSS)
                                    pbtn->setLabel("Add Pattern (LWSS)");
                                else if (selectedPattern == PatternType::Pulsar)
                                    pbtn->setLabel("Add Pattern (Pulsar)");
                                else if (selectedPattern == PatternType::Pentadecathlon)
                                    pbtn->setLabel("Add Pattern (Pentadecathlon)");
                                else if (selectedPattern == PatternType::Acorn)
                                    pbtn->setLabel("Add Pattern (Acorn)");
                                else if (selectedPattern == PatternType::CanadaGoose)
                                    pbtn->setLabel("Add Pattern (CanadaGoose)");
                                else if (selectedPattern == PatternType::Spiral)
                                    pbtn->setLabel("Add Pattern (Spiral)");
                                else if (selectedPattern == PatternType::RPentomino)
                                    pbtn->setLabel("Add Pattern (R-Pentomino)");
                            }
                        }
                    }
                    break;

                default: break;
                }
            }
            else if (auto* me = ev->getIf<sf::Event::MouseButtonPressed>()) {

                int mx = me->position.x;
                int my = me->position.y;

                if (me->button == sf::Mouse::Button::Left &&
                    mx >= 0 && mx < GRID_W &&
                    my >= 0 && my < GRID_N * game.cellSize)
                {
                    sf::Vector2i mp = sf::Mouse::getPosition(window);
                    sf::Vector2f wp = window.mapPixelToCoords(mp);

                    int gx = wp.x / game.cellSize;
                    int gy = wp.y / game.cellSize;

                    if (gx >= 0 && gx < GRID_N && gy >= 0 && gy < GRID_N)
                    {
                        auto& grid = game.getGrid();

                        if (selectedPattern == PatternType::None) {
                            grid[gy][gx] = 1 - grid[gy][gx];
                        }
                        else if (selectedPattern == PatternType::Glider) {
                            Glider().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::Block) {
                            Block().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::GosperGun) {
                            GosperGun().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::SimkinGliderGun) {
                            SimkinGliderGun().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::LWSS) {
                            LWSS().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::Pulsar) {
                            Pulsar().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::Pentadecathlon) {
                            Pentadecathlon().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::Acorn) {
                            Acorn().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::CanadaGoose) {
                            CanadaGoose().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::Spiral) {
                            Spiral().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::RPentomino) {
                            RPentomino().apply(grid, gy, gx);
                        }
                    }
                }
                else if (mx >= GRID_W && me->button == sf::Mouse::Button::Left) {

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
                        case Command::ADD_PATTERN:

                            if (btn->active()) {
                                btn->setActive(false);
                                selectedPattern = PatternType::None;
                            }
                            else {
                                btn->setActive(true);
                                selectedPattern = lastPattern;
                            }
                            break;

                        default: break;
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
                    cell.setPosition({ (float)x * game.cellSize, (float)y * game.cellSize });
                    cell.setFillColor(sf::Color::White);
                    window.draw(cell);
                }
            }
        }

        gridSizeInput.draw(window);
        uiManager.draw(window);
        window.display();
    }
}
