#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Button.h"
#include "Glider.h"
#include "Block.h"
#include "GosperGun.h"

#include "Game.h"
#include "Grid.h"
#include "UIManager.h"
#include "Pattern.h"
#include "WallsManager.h"
#include "InputField.h"


Game::Game(int n) : g(n) {}

Grid& Game::grid() { return g; }
void Game::updateStep() { g.step(); }
void Game::randomize() { g.randomize(); running = false; }
void Game::clear() { g.clear(); running = false; }


void Game::runGame() {

    srand(time(0));

    int GRID_N = 60;

    auto mode = sf::VideoMode::getFullscreenModes()[0];

    WIN_W = mode.size.x;
    WIN_H = mode.size.y;

    MENU_W = WIN_W * 0.20f;
    GRID_W = WIN_W - MENU_W;

    sf::RenderWindow window(mode, "Game of Life", sf::Style::Close);
    window.setFramerateLimit(60);

    Game game(GRID_N);
    game.randomize();

    float cellSize = GRID_W / static_cast<float>(GRID_N);

    WallsManager walls(GRID_N, cellSize, GRID_W);
    game.setWalls(&walls);

    int msPerGen = 150;
    sf::Clock clock;

    sf::RectangleShape cell(sf::Vector2f(cellSize - 1, cellSize - 1));

    sf::RectangleShape menuBg(sf::Vector2f(MENU_W, WIN_H));
    menuBg.setPosition(sf::Vector2f(GRID_W, 0));
    menuBg.setFillColor(sf::Color(50, 50, 50));

    UIManager uiManager(GRID_W, MENU_W);
    uiManager.createButtons();

    InputField gridSizeInput(sf::Vector2f(GRID_W + 20, 720), sf::Vector2f(MENU_W - 40, 50));

    std::vector<PatternType> patternOrder = {
        PatternType::Block,
        PatternType::Glider,
        PatternType::GosperGun
    };

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

                    if (newSize > 5 && newSize < 61) {
                        GRID_N = newSize;

                        float cellSize = GRID_W / (float)GRID_N;

                        game = Game(GRID_N);
                        game.randomize();

                        walls = WallsManager(GRID_N, cellSize, GRID_W);
                        game.setWalls(&walls);
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
                            Button* pbtn = uiManager.getPatternButton();

                            if (pbtn)
                            {
                                if (selectedPattern == PatternType::Block)
                                    pbtn->setLabel("Add Pattern (Block)");
                                else if (selectedPattern == PatternType::Glider)
                                    pbtn->setLabel("Add Pattern (Glider)");
                                else if (selectedPattern == PatternType::GosperGun)
                                    pbtn->setLabel("Add Pattern (Gosper Gun)");
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
                    my >= 0 && my < GRID_N * cellSize)
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
                            Glider().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::Block) {
                            Block().apply(grid, gy, gx);
                        }
                        else if (selectedPattern == PatternType::GosperGun) {
                            GosperGun().apply(grid, gy, gx);
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
                                selectedPattern = PatternType::Block;
                            }
                            break;

                        default: break;
                        }
                    }
                }
            }
        }

        // Update step
        if (game.isRunning() && clock.getElapsedTime().asMilliseconds() > msPerGen) {
            game.updateStep();
            clock.restart();
        }

        // Drawing
        window.clear(sf::Color(30, 30, 30));
        walls.draw(window);

        for (int y = 0; y < GRID_N; y++) {
            for (int x = 0; x < GRID_N; x++) {
                if (game.getGrid()[y][x] == 1) {
                    cell.setPosition({ (float)x * cellSize, (float)y * cellSize });
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
