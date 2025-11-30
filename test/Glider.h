#pragma once
#include "Pattern.h"
#include <vector>

class Glider : public Pattern
{
public:
	Glider();
	void apply(Grid& g, int offsetX, int offsetY) const override;
private:
	std::vector<std::pair<int, int>> cells;
};

