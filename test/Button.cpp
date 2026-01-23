#include "Button.h"
#include <iostream>

// Konstruktor - inicjalizujemy sf::Text przekazan¹ czcionk¹
Button::Button(const std::string& text, const sf::Vector2f& pos, const sf::Vector2f& size, sf::Font& sharedFont)
    : label(sharedFont, text)
{
    // Ustawienia prostok¹ta przycisku
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(100, 100, 100)); // Domyœlny szary
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::White);

    // Ustawienia tekstu
    label.setCharacterSize(18); // Trochê mniejsza czcionka, ¿eby pasowa³a
    label.setFillColor(sf::Color::White);

    // Centrowanie tekstu wewn¹trz przycisku
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    label.setPosition({ pos.x + size.x / 2.f, pos.y + size.y / 2.f });
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(label);
}

bool Button::contains(float x, float y) const {
    return shape.getGlobalBounds().contains({ x, y });
}

// Implementacja setActive - to naprawia b³¹d LNK2019
void Button::setActive(bool a) {
    isActive = a;
    // Aktywny = Zielony, Nieaktywny = Szary
    shape.setFillColor(isActive ? sf::Color(80, 180, 80) : sf::Color(100, 100, 100));
}

void Button::setLabel(const std::string& textStr) {
    label.setString(textStr);

    // Ponowne centrowanie po zmianie tekstu
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    label.setPosition({ shape.getPosition().x + shape.getSize().x / 2.f,
                        shape.getPosition().y + shape.getSize().y / 2.f });
}