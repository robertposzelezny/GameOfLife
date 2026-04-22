#include "InputField.h"
#include <cctype>

InputField::InputField() : active(false) {
    text = nullptr;
}

InputField::InputField(const sf::Vector2f& pos, const sf::Vector2f& size, sf::Font& sharedFont, Mode mode)
    : mode(mode) {
    box.setSize(size);
    box.setPosition(pos);
    box.setFillColor(sf::Color(80, 80, 80));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(2);

    text = std::make_unique<sf::Text>(sharedFont, "");
    text->setCharacterSize(20);
    text->setFillColor(sf::Color::White);
    text->setPosition({ pos.x + 5, pos.y + 5 });
}

void InputField::draw(sf::RenderWindow& window) {
    window.draw(box);
    if (text) {
        window.draw(*text);
    }
}

void InputField::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    if (auto* mouseEv = ev.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mp = window.mapPixelToCoords({ mouseEv->position.x, mouseEv->position.y });
        active = box.getGlobalBounds().contains(mp);
        box.setFillColor(active ? sf::Color(120, 120, 120) : sf::Color(80, 80, 80));
    }

    if (!active || !text) return;

    if (auto* textEv = ev.getIf<sf::Event::TextEntered>()) {
        char c = (char)textEv->unicode;
        if (c == 8) {
            if (!content.empty()) content.pop_back();
        }
        else if (mode == Mode::Numeric) {
            if (std::isdigit(c)) {
                content.push_back(c);
            }
        }
        else {
            if (c >= 32 && c <= 126) {
                content.push_back(c);
            }
        }
        text->setString(content);
    }
}

int InputField::getValue() const {
    if (content.empty()) return -1;
    try {
        return std::stoi(content);
    }
    catch (...) {
        return -1;
    }
}

std::string InputField::getText() const {
    return content;
}

void InputField::setText(const std::string& value) {
    content = value;
    if (text) {
        text->setString(content);
    }
}

void InputField::clear() {
    content.clear();
    if (text) {
        text->setString(content);
    }
}

bool InputField::isActive() const {
    return active;
}