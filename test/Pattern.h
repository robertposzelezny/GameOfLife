#pragma once
#include <vector>
#include <string>
#include "Grid.h"

class Pattern
{
public:
	explicit Pattern(const std::string& patternName = "");
	virtual ~Pattern() = default;

	/**
	 * @brief Adds current selected structure model on grid at clicked grid position.
	 * @param g Grid pointer we want to draw our structure
	 * @param offsetX x position of our grid
	 * @param offsetY y position of our grid
	 */
	void apply(Grid& g, int offsetX, int offsetY) const;

	/**
	 * @brief Loads pattern cells from database
	 * @param patternName name of the pattern to load
	 * @return true if pattern was loaded successfully
	 */
	bool loadFromDatabase(const std::string& patternName);

	/**
	 * @brief Saves current pattern cells to database
	 * @return true if save succeeded
	 */
	bool saveToDatabase() const;

	/**
	 * @brief Deletes current pattern from database
	 * @return true if deletion succeeded
	 */
	bool deleteFromDatabase() const;

	/**
	 * @brief Gets the pattern cells
	 * @return vector of cell coordinates
	 */
	const std::vector<std::pair<int, int>>& getCells() const;

	/**
	 * @brief Sets the pattern name
	 */
	void setPatternName(const std::string& name);

	/**
	 * @brief Gets the pattern name
	 */
	const std::string& getPatternName() const;

protected:
	std::vector<std::pair<int, int>> cells;
	std::string patternName;
};

