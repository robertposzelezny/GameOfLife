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


    /**
     * @brief Draws the button on inserted window
     * @param window Window on which we want to draw our button
     *
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Box check if our current mouse position points the button
     * @param x Current X mouse position
     * @param y Current Y mouse position
     * @return Returns if correct button was clicked or not
     */
    bool contains(float x, float y) const;

    /**
     * @brief Changes style of button to active (green) or default (gray).
     * @param a Bolean value for state of button.
     */
    void setActive(bool a);

    /**
     * @brief Returns current button label.
     *
     */
	std::string getLabel() const { return label.getString(); }

    /**
     * @brief Returns current state of button.
     *
     */
    bool active() const { return isActive; }
};
