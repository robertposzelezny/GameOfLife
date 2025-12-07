#include "UIManager.h"

UIManager::UIManager(int gridW, int menuW)
    : GRID_W(gridW), MENU_W(menuW) {}

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

void UIManager::createButtons() {

    add(new Button("Start/Pause (Space)",
        { GRID_W + 20.f, 50.f }, { MENU_W - 40.f, 50.f }));

    add(new Button("Randomize (R)",
        { GRID_W + 20.f, 130.f }, { MENU_W - 40.f, 50.f }));

    add(new Button("Clear (C)",
        { GRID_W + 20.f, 210.f }, { MENU_W - 40.f, 50.f }));

    add(new Button("Block Left Wall",
        { GRID_W + 20.f, 290.f }, { MENU_W - 40.f, 50.f }));

    add(new Button("Block Right Wall",
        { GRID_W + 20.f, 350.f }, { MENU_W - 40.f, 50.f }));

    add(new Button("Block Top Wall",
        { GRID_W + 20.f, 410.f }, { MENU_W - 40.f, 50.f }));

    add(new Button("Block Bottom Wall",
        { GRID_W + 20.f, 470.f }, { MENU_W - 40.f, 50.f }));

    add(new Button("Add Pattern (Block)",
        { GRID_W + 20.f, 530.f }, { MENU_W - 40.f, 50.f }));

	add(new Button("Space - Start/Pause \n S - Next tick \n R - Randomize \n C - Clear \n Up arrow - Speed up \n Down arrow - Slow down \n Set new grid size:",
		{ GRID_W + 20.f, 590.f }, { MENU_W - 40.f, 200.f }));
}

UIManager::~UIManager() {
    for (auto b : buttons)
        delete b;
}

void UIManager::resetButtons() {
    for (auto b : buttons)
        if (b->getLabel().find("Add Pattern") != std::string::npos)
            b->setActive(false);
}

Command UIManager::getCommand(Button* b) {
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
