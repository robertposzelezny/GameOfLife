#include "InputField.h"
#include <cctype>

InputField::InputField(const sf::Vector2f& pos, const sf::Vector2f& size):text(font,"") {
    box.setSize(size);
    box.setPosition(pos);
    box.setFillColor(sf::Color(80, 80, 80));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(2);

    font.openFromFile("arial.ttf");
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(pos.x + 5, pos.y + 5));
}

void InputField::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}

void InputField::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    if (auto* key = ev.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mp = window.mapPixelToCoords({ key->position.x, key->position.y });
        active = box.getGlobalBounds().contains(mp);
        box.setFillColor(active ? sf::Color(120, 120, 120) : sf::Color(80, 80, 80));
    }

    if (!active) return;

    if (auto* key = ev.getIf<sf::Event::TextEntered>()) {
        char c = (char)key->unicode;

        if (std::isdigit(c)) {
            content.push_back(c);
        }
        else if (c == 8) {
            if (!content.empty())
                content.pop_back();
        }

        text.setString(content);
    }
}

int InputField::getValue() const {
    if (content.empty()) return -1;
    return std::stoi(content);
}
