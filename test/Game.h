#pragma once
#include "Grid.h"
#include "WallsManager.h"
#include "InputField.h"
#include <string>
#include <vector>

class Game {
public:
    explicit Game(int n);

    Grid& grid();
    /**
     * @brief Performs a single simulation step if the game is running.
     */
    void updateStep();
    /**
     * @brief Randomizes the entire grid with alive/dead cells.
     */
    void randomize();

    /**
     * @brief clears current game grid and pauses the game.
     *
     */
    void clear();
    /**
    * @brief Toggles the running state of the simulation.
    */
    void toggleRunning() { running = !running; }
    /**
    * @brief Checks whether the simulation is currently running.
    *
    * @return True if the game is running.
    */
    bool isRunning() const { return running; }
    /**
    *
	* @brief Returns a reference to the game grid.
    */
    Grid& getGrid() { return g; }

    /**
	* @brief Sets the walls manager for the game.
    *
    *
    */
    void setWalls(WallsManager* w) {
        walls = w;
        g.setWalls(w);
    }
    /**
	* @brief Returns a pointer to the walls manager.
    *
    */
    WallsManager* getWalls() const { return walls; }
    /**
	* @brief Runs the main game loop.
    *
    */
    void runGame();

    bool patternPlacementEnabled = false;
    int selectedPatternIndex = 0;
    std::vector<std::string> patternNames;
    float cellSize = 0.f;

private:
    Grid g;
    WallsManager* walls = nullptr;
    bool running = false;
    sf::Font gameFont;
    InputField gridSizeInput;
    int WIN_W = 1100;
    int WIN_H = 800;
    int MENU_W = 300;
    int GRID_W = WIN_W - MENU_W;
};

