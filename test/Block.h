#pragma once
#include <vector>
#include "Pattern.h"

class Block : public Pattern
{
public:
	Block();
	void apply(Grid& g, int offsetX, int offsetY) const override;
private:
	std::vector<std::pair<int, int>> cells;
};

