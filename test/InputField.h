#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

class InputField {
public:
    enum class Mode {
        Numeric,
        Text
    };

    InputField();
    InputField(const sf::Vector2f& pos, const sf::Vector2f& size, sf::Font& sharedFont, Mode mode = Mode::Numeric);

    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    int getValue() const;
    std::string getText() const;
    void setText(const std::string& value);
    void clear();
    bool isActive() const;

private:
    sf::RectangleShape box;
    std::unique_ptr<sf::Text> text;
    bool active = false;
    std::string content;
    Mode mode = Mode::Numeric;
};