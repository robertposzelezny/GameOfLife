#pragma once
#include <vector>
#include "Button.h"
#include "Game.h"

enum class Command {
	START_PAUSE,
	RANDOMIZE,
	CLEAR,
	None,

	BLOCK_LEFT,
	BLOCK_RIGHT,
	BLOCK_TOP,
	BLOCK_BOTTOM,

	ADD_PATTERN
};

class UIManager {
public:
	~UIManager();
	UIManager(int gridW, int menuW);
	/**
	 * @brief Adds a button to the UI.
	 * @param b Pointer to a Button object to be managed.
	 */
	void add(Button* b);
	/**
	 * @brief Returns the button that was clicked at the given mouse coordinates.
	 * @param mx Mouse X position.
	 * @param my Mouse Y position.
	 * @return Pointer to the clicked Button, or nullptr if none was clicked.
	 */
	Button* clicked(float mx, float my);
	/**
	 * @brief Draws all UI buttons to the given render window.
	 * @param win Reference to the RenderWindow.
	 */
	void draw(sf::RenderWindow& win);
	/**
	 * @brief Creates UI buttons.
	 */
	void createButtons();
	/**
	 * @brief Decodes which command corresponds to the given button.
	 * @param b Pointer to a Button.
	 * @return Corresponding Command enum value.
	 */
	Command getCommand(Button* b);
	/**
	 * @brief Resets all buttons to their default visual state.
	 */
	void resetButtons();

	void resetPatternButtons();
	void activatePatternButton(Game::PatternType type);
	Button* getPatternButton();
private:
	int GRID_W;
	int MENU_W;
	std::vector<Button*> buttons;

	
};