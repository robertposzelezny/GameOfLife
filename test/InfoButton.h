#pragma once
#include "Button.h"
#include <SFML/Graphics.hpp>

class InfoButton : public Button {
private:
    bool showInfo = false;
    sf::Font font;

public:
    InfoButton(const std::string& text, const sf::Vector2f& pos, const sf::Vector2f& size);

    void drawInfo(sf::RenderWindow& window, int winW, int winH);
};
