#include "InfoButton.h"

InfoButton::InfoButton(const std::string& text, const sf::Vector2f& pos, const sf::Vector2f& size)
    : Button(text, pos, size)
{
}

void InfoButton::drawInfo(sf::RenderWindow& window, int winW, int winH)
{
    if (!showInfo) return;

    sf::RectangleShape overlay(sf::Vector2f(winW, winH));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::RectangleShape box(sf::Vector2f(600, 400));
    box.setFillColor(sf::Color(40, 40, 40));
    box.setOutlineThickness(3);
    box.setOutlineColor(sf::Color::White);
    box.setPosition(sf::Vector2f((winW - 600) / 2, (winH - 400) / 2));
    window.draw(box);

    auto line = [&](const std::string& str, float y)
        {
            sf::Text t;
			t.setFont(font);
			t.setString(str);
			t.setCharacterSize(22);
            t.setFillColor(sf::Color::White);
            t.setPosition(sf::Vector2f((winW - 600) / 2 + 20, (winH - 400) / 2 + y));
            window.draw(t);
        };

    line("Keyboard Shortcuts:", 20);
    line("Space - Start / Pause simulation", 70);
    line("S     - Next generation (single step)", 110);
    line("R     - Randomize grid", 150);
    line("C     - Clear grid", 190);
    line("Up    - Increase simulation speed", 230);
    line("Down  - Decrease simulation speed", 270);
    line("Left/Right - Change pattern", 310);
    line("ESC   - Close this window", 350);
}
