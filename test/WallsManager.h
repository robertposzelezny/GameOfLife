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
    WallsManager(int gridN, float cellSize, int gridW, float offsetY = 0) : gridN(gridN), cellSize(cellSize) {
        float totalSize = (float)gridN * cellSize;
        float thickness = 2.0f; // U¿yj sta³ej gruboœci zamiast cellSize!

        wallTop.setSize({ totalSize, thickness });
        wallTop.setPosition({ 0, offsetY });

        wallBottom.setSize({ totalSize, thickness });
        wallBottom.setPosition({ 0, offsetY + totalSize - thickness });

        wallLeft.setSize({ thickness, totalSize });
        wallLeft.setPosition({ 0, offsetY });

        wallRight.setSize({ thickness, totalSize });
        wallRight.setPosition({ totalSize - thickness, offsetY });
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
    /**
     * @brief Draws all four wall elements to the specified SFML window.
     * @param window RenderWindow to draw walls on.
     */
    void draw(sf::RenderWindow& window) {
        window.draw(wallLeft);
        window.draw(wallRight);
        window.draw(wallTop);
        window.draw(wallBottom);
    }
    /**
     * @brief Updates wall colors based on their active states.
     */
    void updateColors();
};
