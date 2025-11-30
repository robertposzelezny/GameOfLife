#pragma once
#include <vector>

using Grid = std::vector<std::vector<int>>;

class Pattern
{
public:
	virtual ~Pattern() = default;
	virtual void apply(Grid& g, int offsetX, int offsetY) const = 0;
};

