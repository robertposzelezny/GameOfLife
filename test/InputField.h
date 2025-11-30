#pragma once
#include<SFML/Graphics.hpp>
#include<string>

class InputField
{
public:
	InputField(const sf::Vector2f& pos, const sf::Vector2f& size);
	void draw(sf::RenderWindow& window);
	void handleEvent(const sf::Event& event,sf::RenderWindow& window);
	bool isActive()	const { return active; };
	int getValue() const;

private:
	sf::RectangleShape box;
	sf::Text text;
	sf::Font font;
	bool active = false;
	std::string content;
};

