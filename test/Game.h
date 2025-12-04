#pragma once
#include "Grid.h"
#include "WallsManager.h"

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
    Grid& getGrid() { return g; }

    void setWalls(WallsManager* w) {
        walls = w;
        g.setWalls(w);
    }
    WallsManager* getWalls() const { return walls; }

    void runGame();

    enum class PatternType {
        None,
        Glider,
        Block,
        GosperGun
    };

    PatternType selectedPattern = PatternType::None;

private:
    Grid g;
    WallsManager* walls = nullptr;
    bool running = false;
    int WIN_W = 1100;
    int WIN_H = 800;
    int MENU_W = 300;
    int GRID_W = WIN_W - MENU_W;
};

