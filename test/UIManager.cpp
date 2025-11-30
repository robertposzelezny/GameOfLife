#include "UIManager.h"
#include "UIManager.h"

UIManager::UIManager(int gridW, int menuW) : GRID_W(gridW), MENU_W(menuW) {}

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

    add(new Button("Add Glider",
        { GRID_W + 20.f, 530.f }, { MENU_W - 40.f, 50.f }));

    add(new Button("Add Block",
        { GRID_W + 20.f, 590.f }, { MENU_W - 40.f, 50.f }));

    add(new Button("Add Gosper Glider Gun",
        { GRID_W + 20.f, 650.f }, { MENU_W - 40.f, 50.f }));
}

UIManager::~UIManager() {
    for (auto b : buttons)
        delete b;
}

void UIManager::resetButtons() {
    for (auto b : buttons)
    {
        if (b->getLabel() == "Add Glider" ||
            b->getLabel() == "Add Block" ||
            b->getLabel() == "Add Gosper Glider Gun")
        {
            b->setActive(false);
        }
    }
}