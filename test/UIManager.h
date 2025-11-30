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
	void add(Button* b);
	Button* clicked(float mx, float my);
	void draw(sf::RenderWindow& win);
	void createButtons();
	
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

	void resetButtons();
private:
	int GRID_W;
	int MENU_W;
	std::vector<Button*> buttons;

	
};