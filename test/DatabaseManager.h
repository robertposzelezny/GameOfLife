#pragma once
#include <windows.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include <utility>

class DatabaseManager {
public:
	static DatabaseManager& getInstance();

	/**
	 * @brief Connects to the GameOfLifeDB database
	 * @return true if connection succeeded, false otherwise
	 */
	bool connect();

	/**
	 * @brief Disconnects from the database
	 */
	void disconnect();

	/**
	 * @brief Checks if connected to database
	 * @return true if connected
	 */
	bool isConnected() const;

	/**
	 * @brief Gets pattern data from database
	 * @param patternName name of the pattern
	 * @return vector of cell coordinates as pairs (x, y)
	 */
	std::vector<std::pair<int, int>> getPatternCells(const std::string& patternName);

	/**
	 * @brief Saves pattern to database
	 * @param patternName name of the pattern
	 * @param cells vector of cell coordinates
	 * @return true if save succeeded
	 */
	bool savePattern(const std::string& patternName, const std::vector<std::pair<int, int>>& cells);

	/**
	 * @brief Gets all pattern names from database
	 * @return vector of pattern names
	 */
	std::vector<std::string> getAllPatternNames();

	/**
	 * @brief Saves current board cells to dbo.boards table.
	 * @param cells vector of alive cell coordinates
	 * @return true if save succeeded
	 */
	bool saveBoard(const std::vector<std::pair<int, int>>& cells);

	/**
	 * @brief Gets all saved board ids ordered descending (newest first).
	 * @return vector of board ids
	 */
	std::vector<int> getAllBoardIds();

	/**
	 * @brief Gets board cells by board id.
	 * @param boardId id of board in dbo.GameBoards
	 * @return vector of alive cell coordinates
	 */
	std::vector<std::pair<int, int>> getBoardCells(int boardId);

	/**
	 * @brief Deletes pattern from database
	 * @param patternName name of the pattern
	 * @return true if deletion succeeded
	 */
	bool deletePattern(const std::string& patternName);

private:
	DatabaseManager();
	~DatabaseManager();
	DatabaseManager(const DatabaseManager&) = delete;
	DatabaseManager& operator=(const DatabaseManager&) = delete;
	SQLHENV env;
	SQLHDBC dbc;
	bool connected;
	std::string encodeCells(const std::vector<std::pair<int, int>>& cells) const;
	std::vector<std::pair<int, int>> decodeCells(const std::string& jsonStr) const;
};
