#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

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

#include "ARIAL (1).h"

Game::Game(int n) : g(n), gridSizeInput() {
    if (!gameFont.openFromMemory(ARIAL, ARIAL_SIZE)) {
        std::cerr << "Blad krytyczny: Nie udalo sie zaladowac czcionki z zasobow!" << std::endl;
    }
    running = false;
    cellSize = 0.f;
    selectedPattern = PatternType::None;
    lastPattern = PatternType::Block;
}

void Game::updateStep() {
    g.step();
}

void Game::randomize() {
    g.randomize();
    running = false;
}

void Game::clear() {
    g.clear();
    running = false;
}

Grid& Game::grid() {
    return g;
}

void Game::runGame() {
    srand(static_cast<unsigned int>(time(0)));

    auto mode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(mode, "Game of Life", sf::Style::None);
    window.setFramerateLimit(60);

    WIN_W = (float)window.getSize().x;
    WIN_H = (float)window.getSize().y;
    MENU_W = WIN_W * 0.25f;
    GRID_W = WIN_W - MENU_W;

    int GRID_N = 100;
    g.resize(GRID_N);

    float sizeW = GRID_W / static_cast<float>(GRID_N);
    float sizeH = WIN_H / static_cast<float>(GRID_N);
    cellSize = std::min(GRID_W / (float)GRID_N, WIN_H / (float)GRID_N);

    float gridHeight = GRID_N * cellSize;
    float offsetY = (WIN_H - gridHeight) / 2.0f;

    WallsManager wallsObj(GRID_N, cellSize, (int)GRID_W, offsetY);
    setWalls(&wallsObj);

    UIManager uiManager(GRID_W, MENU_W);
    uiManager.createButtons(gameFont);
    this->gridSizeInput = InputField(sf::Vector2f(GRID_W + 20, WIN_H - 100), sf::Vector2f(MENU_W - 40, 40), gameFont);

    auto findPatternIndex = [&](PatternType t) {
        for (size_t i = 0; i < patternOrder.size(); i++)
            if (patternOrder[i] == t) return static_cast<int>(i);
        return -1;
        };

    sf::Clock clock;
    int msPerGen = 150;

    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) {

            gridSizeInput.handleEvent(*ev, window);

            if (ev->is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            if (auto* ke = ev->getIf<sf::Event::KeyPressed>()) {
                using K = sf::Keyboard::Key;
                switch (ke->code) {
                case K::Space: toggleRunning(); break;
                case K::R: randomize(); break;
                case K::C: clear(); break;
                case K::S: updateStep(); break;
                case K::Up: msPerGen = std::max(10, msPerGen - 20); break;
                case K::Down: msPerGen += 20; break;
                case K::Escape:window.close();break;
                case K::Enter: {
                    int newSize = gridSizeInput.getValue();
                    if (newSize >= 5 && newSize <= 500) {
                        GRID_N = newSize;
                        g.resize(GRID_N);
                        cellSize = std::min(GRID_W / (float)GRID_N, WIN_H / (float)GRID_N);
                        float gridHeight = GRID_N * cellSize;
                        offsetY = (WIN_H - gridHeight) / 2.0f;

                        wallsObj = WallsManager(GRID_N, cellSize, (int)GRID_W, offsetY);
                        setWalls(&wallsObj);
                    }
                    break;
                }
                case K::Left:
                case K::Right:
                    if (selectedPattern != PatternType::None) {
                        int idx = findPatternIndex(selectedPattern);
                        if (idx != -1) {
                            if (ke->code == K::Right) idx++; else idx--;
                            if (idx < 0) idx = (int)patternOrder.size() - 1;
                            if (idx >= (int)patternOrder.size()) idx = 0;

                            selectedPattern = patternOrder[idx];
                            lastPattern = selectedPattern;

                            Button* pbtn = uiManager.getPatternButton();
                            if (pbtn) {
                                std::string name = "Unknown";
                                if (selectedPattern == PatternType::Block) name = "Block";
                                else if (selectedPattern == PatternType::Glider) name = "Glider";
                                else if (selectedPattern == PatternType::GosperGun) name = "Gosper Gun";
                                else if (selectedPattern == PatternType::SimkinGliderGun) name = "SGG";
                                else if (selectedPattern == PatternType::LWSS) name = "LWSS";
                                else if (selectedPattern == PatternType::Pulsar) name = "Pulsar";
                                else if (selectedPattern == PatternType::Pentadecathlon) name = "Pentadecathlon";
                                else if (selectedPattern == PatternType::Acorn) name = "Acorn";
                                else if (selectedPattern == PatternType::CanadaGoose) name = "CanadaGoose";
                                else if (selectedPattern == PatternType::Spiral) name = "Spiral";
                                else if (selectedPattern == PatternType::RPentomino) name = "R-Pentomino";
                                pbtn->setLabel("Add Pattern (" + name + ")");
                            }
                        }
                    }
                    break;
                default: break;
                }
            }

            if (auto* me = ev->getIf<sf::Event::MouseButtonPressed>()) {
                float mx = (float)me->position.x;
                float my = (float)me->position.y;

                if (me->button == sf::Mouse::Button::Left) {
                    if (mx < GRID_W) {
                        float relativeY = my - offsetY;

                        int gx = static_cast<int>(mx / cellSize);
                        int gy = static_cast<int>(relativeY / cellSize);

                        if (gx >= 0 && gx < GRID_N && gy >= 0 && gy < (int)g.getCells().size()) {
                            if (selectedPattern == PatternType::None) {
                                auto& gridCells = g.getCells();
                                gridCells[gy][gx] = 1 - gridCells[gy][gx];
                            }
                            else {
                                if (selectedPattern == PatternType::Glider) Glider().apply(g, gy, gx);
                                else if (selectedPattern == PatternType::Block) Block().apply(g, gy, gx);
                                else if (selectedPattern == PatternType::GosperGun) GosperGun().apply(g, gy, gx);
                                else if (selectedPattern == PatternType::SimkinGliderGun) SimkinGliderGun().apply(g, gy, gx);
                                else if (selectedPattern == PatternType::LWSS) LWSS().apply(g, gy, gx);
                                else if (selectedPattern == PatternType::Pulsar) Pulsar().apply(g, gy, gx);
                                else if (selectedPattern == PatternType::Pentadecathlon) Pentadecathlon().apply(g, gy, gx);
                                else if (selectedPattern == PatternType::Acorn) Acorn().apply(g, gy, gx);
                                else if (selectedPattern == PatternType::CanadaGoose) CanadaGoose().apply(g, gy, gx);
                                else if (selectedPattern == PatternType::Spiral) Spiral().apply(g, gy, gx);
                                else if (selectedPattern == PatternType::RPentomino) RPentomino().apply(g, gy, gx);
                            }
                        }
                    }
                    else {
                        Button* btn = uiManager.clicked(mx, my);
                        if (btn) {
                            switch (uiManager.getCommand(btn)) {
                            case Command::START_PAUSE: toggleRunning(); break;
                            case Command::RANDOMIZE: randomize(); break;
                            case Command::CLEAR: clear(); break;
                            case Command::BLOCK_LEFT: wallsObj.toggleLeft(); btn->setActive(wallsObj.left()); break;
                            case Command::BLOCK_RIGHT: wallsObj.toggleRight(); btn->setActive(wallsObj.right()); break;
                            case Command::BLOCK_TOP: wallsObj.toggleTop(); btn->setActive(wallsObj.top()); break;
                            case Command::BLOCK_BOTTOM: wallsObj.toggleBottom(); btn->setActive(wallsObj.bottom()); break;
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
        }

        if (isRunning() && clock.getElapsedTime().asMilliseconds() > msPerGen) {
            updateStep();
            clock.restart();
        }

        window.clear(sf::Color(30, 30, 30));
        wallsObj.draw(window);

        sf::RectangleShape cellRect(sf::Vector2f(cellSize - 1.f, cellSize - 1.f));
        cellRect.setFillColor(sf::Color::White);
        auto& drawGrid = g.getCells();
        for (int y = 0; y < (int)drawGrid.size(); y++) {
            for (int x = 0; x < (int)drawGrid[y].size(); x++) {
                if (drawGrid[y][x] == 1) {
                    cellRect.setPosition({ x * cellSize, y * cellSize + offsetY });
                    window.draw(cellRect);
                }
            }
        }

        uiManager.draw(window);
        gridSizeInput.draw(window);
        window.display();
    }
}