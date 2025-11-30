#pragma once
#include <SFML/Graphics.hpp>

class WallsManager {
private:
    float cellSize;
    int gridN;

    bool blockLeft = false;
    bool blockRight = false;
    bool blockTop = false;
    bool blockBottom = false;

    sf::RectangleShape wallLeft;
    sf::RectangleShape wallRight;
    sf::RectangleShape wallTop;
    sf::RectangleShape wallBottom;

public:
    WallsManager(int gridN, float cellSize, int gridW) : gridN(gridN), cellSize(cellSize) {
        wallLeft.setSize({ cellSize, gridN * cellSize });
        wallLeft.setPosition({ 0, 0 });
        wallLeft.setFillColor(sf::Color::Transparent);

        wallRight.setSize({ cellSize, gridN * cellSize });
        wallRight.setPosition({ gridW - cellSize, 0 });
        wallRight.setFillColor(sf::Color::Transparent);

        wallTop.setSize({ gridN * cellSize, cellSize });
        wallTop.setPosition({ 0, 0 });
        wallTop.setFillColor(sf::Color::Transparent);

        wallBottom.setSize({ gridN * cellSize, cellSize });
        wallBottom.setPosition({ 0, gridN * cellSize - cellSize });
        wallBottom.setFillColor(sf::Color::Transparent);
    }

    bool left() const { return blockLeft; }
    bool right() const { return blockRight; }
    bool top() const { return blockTop; }
    bool bottom() const { return blockBottom; }

    void toggleLeft() {
        blockLeft = !blockLeft;
        wallLeft.setFillColor(blockLeft ? sf::Color(200, 50, 50) : sf::Color::Transparent);
    }
    void toggleRight() {
        blockRight = !blockRight;
        wallRight.setFillColor(blockRight ? sf::Color(200, 50, 50) : sf::Color::Transparent);
    }
    void toggleTop() {
        blockTop = !blockTop;
        wallTop.setFillColor(blockTop ? sf::Color(200, 50, 50) : sf::Color::Transparent);
    }
    void toggleBottom() {
        blockBottom = !blockBottom;
        wallBottom.setFillColor(blockBottom ? sf::Color(200, 50, 50) : sf::Color::Transparent);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(wallLeft);
        window.draw(wallRight);
        window.draw(wallTop);
        window.draw(wallBottom);
    }

    void updateColors();
};
