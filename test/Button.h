#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text label;
    sf::Font font;

    bool isActive = false;
	
public:
    Button(const std::string& text, const sf::Vector2f& pos, const sf::Vector2f& size);

    void draw(sf::RenderWindow& window);
    bool contains(float x, float y) const;
    void setActive(bool a) {
        isActive = a;
        shape.setFillColor(isActive ? sf::Color(80, 180, 80) : sf::Color(100, 100, 100));
    }

    bool active() const { return isActive; }
};
