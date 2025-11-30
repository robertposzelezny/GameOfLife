#pragma once
#include "Pattern.h"
#include <vector>

class GosperGun : public Pattern
{
public:
	GosperGun();
	void apply(Grid& g, int offsetX, int offsetY) const override;
private:
	std::vector<std::pair<int, int>> cells;
};

