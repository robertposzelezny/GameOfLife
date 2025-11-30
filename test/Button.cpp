#include "Button.h"
#include <iostream>


Button::Button(const std::string& text, const sf::Vector2f& pos, const sf::Vector2f& size) : label(font,text)
{
    if (!font.openFromFile("arial.ttf")) {
        std::cout << "Font load FAILED!\n";
    }

    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(100, 100, 100));

    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(22);
    label.setFillColor(sf::Color::White);

    sf::FloatRect bounds = label.getLocalBounds();

    label.setOrigin(sf::Vector2f(
        bounds.size.x / 2.f,
        bounds.size.y / 2.f
    ));

    label.setPosition(sf::Vector2f(
        pos.x + size.x / 2.f,
        pos.y + size.y / 2.f
    ));
}

void Button::draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(label);
}

bool Button::contains(float x, float y) const
{
    return shape.getGlobalBounds().contains(sf::Vector2f(x, y));
}

void Button::setActive(bool a)
{
	isActive = a;
    shape.setFillColor(isActive ? sf::Color(80, 180, 80) : sf::Color(100, 100, 100));
}