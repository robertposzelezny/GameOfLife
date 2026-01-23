#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text label;
    bool isActive = false;

public:
    Button(const std::string& text, const sf::Vector2f& pos, const sf::Vector2f& size, sf::Font& sharedFont);

    /**
     * @brief Rysuje przycisk w oknie.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Sprawdza, czy myszka znajduje siê nad przyciskiem.
     */
    bool contains(float x, float y) const;

    /**
     * @brief Zmienia stan aktywnoœci przycisku (kolor zielony/szary).
     */
    void setActive(bool a);

    /**
     * @brief Zwraca tekst etykiety.
     */
    std::string getLabel() const { return label.getString(); }

    /**
     * @brief Zwraca stan przycisku.
     */
    bool active() const { return isActive; }

    /**
     * @brief Ustawia now¹ treœæ etykiety.
     */
    void setLabel(const std::string& text);
};