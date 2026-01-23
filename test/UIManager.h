#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Button.h"

enum class Command {
    None, START_PAUSE, RANDOMIZE, CLEAR,
    BLOCK_LEFT, BLOCK_RIGHT, BLOCK_TOP, BLOCK_BOTTOM,
    ADD_PATTERN
};

class UIManager {
private:
    std::vector<Button*> buttons;
    int GRID_W;
    int MENU_W;

public:
    UIManager(int gridW, int menuW);
    ~UIManager();

    void add(Button* b);
    void createButtons(sf::Font& sharedFont);
    void draw(sf::RenderWindow& window);

    Button* clicked(float mx, float my);
    void resetButtons();

    Command getCommand(Button* b);
    Button* getPatternButton();
};