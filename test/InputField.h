// InputField.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory> // Potrzebne dla unique_ptr

class InputField {
public:
    InputField();
    InputField(const sf::Vector2f& pos, const sf::Vector2f& size, sf::Font& sharedFont);

    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    int getValue() const;

private:
    sf::RectangleShape box;
    std::unique_ptr<sf::Text> text; // Zmiana na wskaünik
    bool active = false;
    std::string content;
};