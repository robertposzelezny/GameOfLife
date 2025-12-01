#pragma once
#include "Pattern.h"
#include <vector>

class Glider : public Pattern
{
public:
	Glider();
	/**
	 * @brief Adds current selected structure model on grid at clicked grid position.
	 * @param g Grid pointer we want to draw our structure
	 * @param offsetX x position of our grid
	 * @param offsetY y position of our grid
	 */
	void apply(Grid& g, int offsetX, int offsetY) const override;
private:
	std::vector<std::pair<int, int>> cells;
};

