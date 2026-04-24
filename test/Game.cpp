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
#include "DatabaseManager.h"
#include "BuiltInPatterns.h"

#include "Pattern.h"

#include "ARIAL (1).h"

Game::Game(int n) : g(n), gridSizeInput() {
    if (!gameFont.openFromMemory(ARIAL, ARIAL_SIZE)) {
        std::cerr << "Blad krytyczny: Nie udalo sie zaladowac czcionki z zasobow!" << std::endl;
    }
    running = false;
    cellSize = 0.f;
    patternPlacementEnabled = false;
    selectedPatternIndex = 0;
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

    cellSize = std::min(GRID_W / (float)GRID_N, WIN_H / (float)GRID_N);

    float gridHeight = GRID_N * cellSize;
    float offsetY = (WIN_H - gridHeight) / 2.0f;

    WallsManager wallsObj(GRID_N, cellSize, (int)GRID_W, offsetY);
    setWalls(&wallsObj);

    UIManager uiManager(GRID_W, MENU_W);
    DatabaseManager& db = DatabaseManager::getInstance();
    patternNames = db.getAllPatternNames();
    boardIds = db.getAllBoardIds();
    selectedBoardIndex = 0;
    if (patternNames.empty()) {
        for (const auto& pattern : getBuiltInPatterns()) {
            patternNames.push_back(pattern.first);
        }
    }

    uiManager.createButtons(gameFont, patternNames);
    this->gridSizeInput = InputField(sf::Vector2f(GRID_W + 20, WIN_H - 100), sf::Vector2f(MENU_W - 40, 40), gameFont);
    this->patternNameInput = InputField(sf::Vector2f(GRID_W + 20, 650.f), sf::Vector2f(MENU_W - 40, 40), gameFont, InputField::Mode::Text);
    patternNameInput.setText("NewPattern");

    auto selectedPatternName = [&]() -> std::string {
        if (patternNames.empty()) {
            return "None";
        }
        if (selectedPatternIndex < 0 || selectedPatternIndex >= static_cast<int>(patternNames.size())) {
            selectedPatternIndex = 0;
        }
        return patternNames[selectedPatternIndex];
    };

    auto updatePatternToggle = [&]() {
        Button* pbtn = uiManager.getPatternToggleButton();
        if (!pbtn) {
            return;
        }

        pbtn->setLabel("Add Pattern (" + selectedPatternName() + ")");
        pbtn->setActive(patternPlacementEnabled);
    };

    auto selectedBoardId = [&]() -> int {
        if (boardIds.empty()) {
            return -1;
        }
        if (selectedBoardIndex < 0 || selectedBoardIndex >= static_cast<int>(boardIds.size())) {
            selectedBoardIndex = 0;
        }
        return boardIds[selectedBoardIndex];
    };

    auto updateLoadBoardLabel = [&]() {
        Button* loadBtn = uiManager.getLoadBoardButton();
        if (!loadBtn) {
            return;
        }

        int boardId = selectedBoardId();
        if (boardId < 0) {
            loadBtn->setLabel("Load Board (none)");
        }
        else {
            loadBtn->setLabel("Load Board (#" + std::to_string(boardId) + ")");
        }
    };

    auto collectLiveCells = [&](bool normalize) -> std::vector<std::pair<int, int>> {
        std::vector<std::pair<int, int>> liveCells;
        auto& cells = g.getCells();
        int minX = 999999;
        int minY = 999999;

        for (int y = 0; y < static_cast<int>(cells.size()); ++y) {
            for (int x = 0; x < static_cast<int>(cells[y].size()); ++x) {
                if (cells[y][x] == 1) {
                    liveCells.push_back({ x, y });
                    if (x < minX) minX = x;
                    if (y < minY) minY = y;
                }
            }
        }

        if (normalize) {
            for (auto& cell : liveCells) {
                cell.first -= minX;
                cell.second -= minY;
            }
        }

        return liveCells;
    };

    auto applyBoardCells = [&](const std::vector<std::pair<int, int>>& liveCells) {
        g.clear();
        auto& gridCells = g.getCells();
        const int gridSize = static_cast<int>(gridCells.size());
        for (const auto& cell : liveCells) {
            const int x = cell.first;
            const int y = cell.second;
            if (x >= 0 && x < gridSize && y >= 0 && y < gridSize) {
                gridCells[y][x] = 1;
            }
        }
        running = false;
    };

    updatePatternToggle();
    updateLoadBoardLabel();

    sf::Clock clock;
    int msPerGen = 150;

    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) {

            gridSizeInput.handleEvent(*ev, window);
            patternNameInput.handleEvent(*ev, window);

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
                    if (gridSizeInput.isActive()) {
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
                    }
                    break;
                }
                case K::Left:
                case K::Right:
                    if (patternPlacementEnabled && !patternNames.empty()) {
                        if (ke->code == K::Right) {
                            selectedPatternIndex++;
                        }
                        else {
                            selectedPatternIndex--;
                        }

                        if (selectedPatternIndex < 0) {
                            selectedPatternIndex = static_cast<int>(patternNames.size()) - 1;
                        }
                        if (selectedPatternIndex >= static_cast<int>(patternNames.size())) {
                            selectedPatternIndex = 0;
                        }

                        updatePatternToggle();
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
                            if (!patternPlacementEnabled) {
                                auto& gridCells = g.getCells();
                                gridCells[gy][gx] = 1 - gridCells[gy][gx];
                            }
                            else {
                                if (!patternNames.empty()) {
                                    Pattern pattern(selectedPatternName());
                                    pattern.apply(g, gy, gx);
                                }
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
                                    patternPlacementEnabled = false;
                                }
                                else {
                                    patternPlacementEnabled = true;
                                }
                                updatePatternToggle();
                                break;
                            case Command::SAVE_PATTERN: {
                                std::string newPatternName = patternNameInput.getText();
                                newPatternName.erase(0, newPatternName.find_first_not_of(" \t"));
                                if (!newPatternName.empty()) {
                                    newPatternName.erase(newPatternName.find_last_not_of(" \t") + 1);
                                }

                                const auto liveCells = collectLiveCells(true);
                                if (newPatternName.empty()) {
                                    std::cerr << "Pattern name cannot be empty." << std::endl;
                                }
                                else if (liveCells.empty()) {
                                    std::cerr << "Grid has no alive cells to save as pattern." << std::endl;
                                }
                                else if (!db.savePattern(newPatternName, liveCells)) {
                                    std::cerr << "Failed to save pattern '" << newPatternName << "'." << std::endl;
                                }
                                else {
                                    bool alreadyListed = false;
                                    for (const auto& name : patternNames) {
                                        if (name == newPatternName) {
                                            alreadyListed = true;
                                            break;
                                        }
                                    }
                                    if (!alreadyListed) {
                                        patternNames.push_back(newPatternName);
                                    }

                                    for (size_t i = 0; i < patternNames.size(); ++i) {
                                        if (patternNames[i] == newPatternName) {
                                            selectedPatternIndex = static_cast<int>(i);
                                            break;
                                        }
                                    }

                                    patternPlacementEnabled = true;
                                    updatePatternToggle();
                                    std::cout << "Saved pattern '" << newPatternName << "' to database." << std::endl;
                                }
                                break;
                            }
                            case Command::SAVE_BOARD: {
                                const auto liveCells = collectLiveCells(false);
                                if (liveCells.empty()) {
                                    std::cerr << "Grid has no alive cells to save as board." << std::endl;
                                }
                                else if (!db.saveBoard(liveCells)) {
                                    std::cerr << "Failed to save board." << std::endl;
                                }
                                else {
                                    boardIds = db.getAllBoardIds();
                                    selectedBoardIndex = 0;
                                    updateLoadBoardLabel();
                                    std::cout << "Saved current board to database." << std::endl;
                                }
                                break;
                            }
                            case Command::PREV_BOARD:
                                if (!boardIds.empty()) {
                                    selectedBoardIndex--;
                                    if (selectedBoardIndex < 0) {
                                        selectedBoardIndex = static_cast<int>(boardIds.size()) - 1;
                                    }
                                    updateLoadBoardLabel();
                                }
                                break;
                            case Command::NEXT_BOARD:
                                if (!boardIds.empty()) {
                                    selectedBoardIndex++;
                                    if (selectedBoardIndex >= static_cast<int>(boardIds.size())) {
                                        selectedBoardIndex = 0;
                                    }
                                    updateLoadBoardLabel();
                                }
                                break;
                            case Command::LOAD_BOARD: {
                                const int boardId = selectedBoardId();
                                if (boardId < 0) {
                                    std::cerr << "No saved boards found in database." << std::endl;
                                }
                                else {
                                    const auto savedCells = db.getBoardCells(boardId);
                                    if (savedCells.empty()) {
                                        std::cerr << "Saved board #" << boardId << " is empty or unavailable." << std::endl;
                                    }
                                    else {
                                        applyBoardCells(savedCells);
                                        std::cout << "Loaded board #" << boardId << " from database." << std::endl;
                                    }
                                }
                                break;
                            }
                            case Command::SELECT_PATTERN: {
                                const std::string clickedPattern = uiManager.getPatternName(btn);
                                for (size_t i = 0; i < patternNames.size(); ++i) {
                                    if (patternNames[i] == clickedPattern) {
                                        selectedPatternIndex = static_cast<int>(i);
                                        patternPlacementEnabled = true;
                                        break;
                                    }
                                }
                                updatePatternToggle();
                                break;
                            }
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
        patternNameInput.draw(window);
        gridSizeInput.draw(window);
        window.display();
    }
}