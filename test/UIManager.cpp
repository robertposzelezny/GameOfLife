#include "UIManager.h"

UIManager::UIManager(int gridW, int menuW)
    : GRID_W(gridW), MENU_W(menuW) {}

UIManager::~UIManager() {
    for (auto b : buttons)
        delete b;
    buttons.clear();
}

void UIManager::add(Button* b) {
    buttons.push_back(b);
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

void UIManager::createButtons(sf::Font& sharedFont) {
    add(new Button("Start/Pause (Space)",
        { (float)GRID_W + 20.f, 50.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont));

    add(new Button("Randomize (R)",
        { (float)GRID_W + 20.f, 130.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont));

    add(new Button("Clear (C)",
        { (float)GRID_W + 20.f, 210.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont));

    add(new Button("Block Left Wall",
        { (float)GRID_W + 20.f, 290.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont));

    add(new Button("Block Right Wall",
        { (float)GRID_W + 20.f, 350.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont));

    add(new Button("Block Top Wall",
        { (float)GRID_W + 20.f, 410.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont));

    add(new Button("Block Bottom Wall",
        { (float)GRID_W + 20.f, 470.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont));

    add(new Button("Add Pattern (Block)",
        { (float)GRID_W + 20.f, 530.f }, { (float)MENU_W - 40.f, 50.f }, sharedFont));

    add(new Button("Space - Start/Pause \n S - Next tick \n R - Randomize \n C - Clear \n Up arrow - Speed up \n Down arrow - Slow down \n ESC - Close App \n InputField[Below] - Set new grid size ",
        { (float)GRID_W + 20.f, 590.f }, { (float)MENU_W - 40.f, 200.f }, sharedFont));
}

void UIManager::resetButtons() {
    for (auto b : buttons)
        if (b->getLabel().find("Add Pattern") != std::string::npos)
            b->setActive(false);
}

Command UIManager::getCommand(Button* b) {
    if (!b) return Command::None;
    std::string lbl = b->getLabel();

    if (lbl == "Start/Pause (Space)") return Command::START_PAUSE;
    if (lbl == "Randomize (R)") return Command::RANDOMIZE;
    if (lbl == "Clear (C)") return Command::CLEAR;

    if (lbl == "Block Left Wall") return Command::BLOCK_LEFT;
    if (lbl == "Block Right Wall") return Command::BLOCK_RIGHT;
    if (lbl == "Block Top Wall") return Command::BLOCK_TOP;
    if (lbl == "Block Bottom Wall") return Command::BLOCK_BOTTOM;

    if (lbl.rfind("Add Pattern", 0) == 0)
        return Command::ADD_PATTERN;

    return Command::None;
}

Button* UIManager::getPatternButton() {
    for (auto b : buttons) {
        if (b->getLabel().find("Add Pattern") != std::string::npos)
            return b;
    }
    return nullptr;
}