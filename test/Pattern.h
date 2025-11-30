#pragma once
#include <vector>
#include "Grid.h"

class Pattern
{
public:
	virtual ~Pattern() = default;
	virtual void apply(Grid& g, int offsetX, int offsetY) const = 0;
};

