#pragma once
#include<SFML/Graphics.hpp>
#include<string>

class InputField
{
public:
	InputField(const sf::Vector2f& pos, const sf::Vector2f& size);
	/**
	 * @brief Draws the input field onto the given window.
	 *
	 * @param window Reference to the RenderWindow.
	 */
	void draw(sf::RenderWindow& window);
	/**
	 * @brief Handles SFML events such as mouse clicks and text input.
	 * @param event to process.
	 * @param window Reference to the RenderWindow.
	 */
	void handleEvent(const sf::Event& event,sf::RenderWindow& window);
	/**
	* @brief Checks whether the input field is currently active.
	*
	* @return True if the field is active.
	*/
	bool isActive()	const { return active; };
	/**
	 * @brief Gets the integer value of the input text.
	 */
	int getValue() const;

private:
	sf::RectangleShape box;
	sf::Text text;
	sf::Font font;
	bool active = false;
	std::string content;
};

