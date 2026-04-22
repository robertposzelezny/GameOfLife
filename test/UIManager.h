#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Button.h"

enum class Command {
    None, START_PAUSE, RANDOMIZE, CLEAR,
    BLOCK_LEFT, BLOCK_RIGHT, BLOCK_TOP, BLOCK_BOTTOM,
    ADD_PATTERN,
    SELECT_PATTERN
};

class UIManager {
private:
    std::vector<Button*> buttons;
    std::unordered_map<Button*, Command> buttonCommands;
    std::unordered_map<Button*, std::string> buttonPatterns;
    Button* patternToggleButton = nullptr;
    int GRID_W;
    int MENU_W;

public:
    UIManager(int gridW, int menuW);
    ~UIManager();

    void add(Button* b, Command cmd = Command::None, const std::string& patternName = "");
    void createButtons(sf::Font& sharedFont, const std::vector<std::string>& patternNames);
    void draw(sf::RenderWindow& window);

    Button* clicked(float mx, float my);
    void resetButtons();

    Command getCommand(Button* b);
    Button* getPatternToggleButton();
    std::string getPatternName(Button* b) const;
};