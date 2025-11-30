#include "WallsManager.h"


void WallsManager::updateColors() {
	wallLeft.setFillColor(blockLeft ? sf::Color(200, 50, 50) : sf::Color::Transparent);
	wallRight.setFillColor(blockRight ? sf::Color(200, 50, 50) : sf::Color::Transparent);
	wallTop.setFillColor(blockTop ? sf::Color(200, 50, 50) : sf::Color::Transparent);
	wallBottom.setFillColor(blockBottom ? sf::Color(200, 50, 50) : sf::Color::Transparent);
}