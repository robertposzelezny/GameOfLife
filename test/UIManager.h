#pragma once
#include <vector>
#include "Button.h"

enum Command {
	START_PAUSE,
	RANDOMIZE,
	CLEAR,
	BLOCK_LEFT,
	BLOCK_RIGHT,
	BLOCK_TOP,
	BLOCK_BOTTOM,
	ADD_GLIDER,
	ADD_BLOCK,
	ADD_GOSPER_GUN
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
	Command getCommand(Button* b) {
		if (b == buttons[0]) return START_PAUSE;
		if (b == buttons[1]) return RANDOMIZE;
		if (b == buttons[2]) return CLEAR;
		if (b == buttons[3]) return BLOCK_LEFT;
		if (b == buttons[4]) return BLOCK_RIGHT;
		if (b == buttons[5]) return BLOCK_TOP;
		if (b == buttons[6]) return BLOCK_BOTTOM;
		if (b == buttons[7]) return ADD_GLIDER;
		if (b == buttons[8]) return ADD_BLOCK;
		if (b == buttons[9]) return ADD_GOSPER_GUN;
		return START_PAUSE;
	}
	/**
	 * @brief Resets all buttons to their default visual state.
	 */
	void resetButtons();
private:
	int GRID_W;
	int MENU_W;
	std::vector<Button*> buttons;

	
};