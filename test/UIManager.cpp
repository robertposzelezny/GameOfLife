#include "UIManager.h"

UIManager::UIManager(int gridW, int menuW)
    : GRID_W(gridW), MENU_W(menuW) {}

UIManager::~UIManager() {
    for (auto b : buttons)
        delete b;
    buttons.clear();
    buttonCommands.clear();
    buttonPatterns.clear();
}

void UIManager::add(Button* b, Command cmd, const std::string& patternName) {
    buttons.push_back(b);
    buttonCommands[b] = cmd;
    if (!patternName.empty()) {
        buttonPatterns[b] = patternName;
    }
}

Button* UIManager::clicked(float mx, float my) {
    for (auto b : buttons)
        if (b->contains(mx, my))
            return b;
    return nullptr;
}

void UIManager::draw(sf::RenderWindow& window) {
    for (auto b : buttons)
        b->draw(window);
}

void UIManager::createButtons(sf::Font& sharedFont, const std::vector<std::string>& patternNames) {
    add(new Button("Start/Pause (Space)",
        { (float)GRID_W + 20.f, 50.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont), Command::START_PAUSE);

    add(new Button("Randomize (R)",
        { (float)GRID_W + 20.f, 130.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont), Command::RANDOMIZE);

    add(new Button("Clear (C)",
        { (float)GRID_W + 20.f, 210.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont), Command::CLEAR);

    add(new Button("Block Left Wall",
        { (float)GRID_W + 20.f, 290.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont), Command::BLOCK_LEFT);

    add(new Button("Block Right Wall",
        { (float)GRID_W + 20.f, 350.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont), Command::BLOCK_RIGHT);

    add(new Button("Block Top Wall",
        { (float)GRID_W + 20.f, 410.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont), Command::BLOCK_TOP);

    add(new Button("Block Bottom Wall",
        { (float)GRID_W + 20.f, 470.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont), Command::BLOCK_BOTTOM);

    std::string initialPatternName = "None";
    if (!patternNames.empty()) {
        initialPatternName = patternNames.front();
    }

    patternToggleButton = new Button("Add Pattern (" + initialPatternName + ")",
        { (float)GRID_W + 20.f, 530.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont);
    add(patternToggleButton, Command::ADD_PATTERN);

    add(new Button("Save Pattern",
        { (float)GRID_W + 20.f, 590.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont), Command::SAVE_PATTERN);

    add(new Button("Save Board",
        { (float)GRID_W + 20.f, 700.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont), Command::SAVE_BOARD);

    add(new Button("Space - Start/Pause \n S - Next tick \n R - Randomize \n C - Clear \n Arrows L/R - Change pattern \n ESC - Close App",
        { (float)GRID_W + 20.f, 760.f }, { (float)MENU_W - 40.f, 90.f }, sharedFont));
}

void UIManager::resetButtons() {
    if (patternToggleButton) {
        patternToggleButton->setActive(false);
        patternToggleButton->setLabel("Add Pattern");
    }
}

Command UIManager::getCommand(Button* b) {
    if (!b) {
        return Command::None;
    }

    const auto it = buttonCommands.find(b);
    if (it == buttonCommands.end()) {
        return Command::None;
    }

    return it->second;
}

Button* UIManager::getPatternToggleButton() {
    return patternToggleButton;
}

std::string UIManager::getPatternName(Button* b) const {
    const auto it = buttonPatterns.find(b);
    if (it != buttonPatterns.end()) {
        return it->second;
    }
    return "";
}