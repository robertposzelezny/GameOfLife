#include "DatabaseManager.h"
#include "nanodbc.h"

#include <iostream>
#include <sstream>

DatabaseManager& DatabaseManager::getInstance() {
	static DatabaseManager instance;
	return instance;
}

DatabaseManager::DatabaseManager() : connected(false) {}

DatabaseManager::~DatabaseManager() {
	disconnect();
}

bool DatabaseManager::connect() {
	if (connected) return true;
	try {
		conn = std::make_unique<nanodbc::connection>(
			std::string(
				"DRIVER={SQL Server};"
				"SERVER=DESKTOP-TQN13PE;"
				"DATABASE=GameOfLifeDB;"
				"Trusted_Connection=yes;"));
		connected = true;
		std::cout << "Connected to database!" << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Connection failed: " << e.what() << std::endl;
		return false;
	}
}

void DatabaseManager::disconnect() {
	conn.reset();
	connected = false;
}

bool DatabaseManager::isConnected() const {
	return connected && conn && conn->connected();
}

std::string DatabaseManager::encodeCells(const std::vector<std::pair<int, int>>& cells) const {
	std::stringstream ss;
	ss << "[";
	for (size_t i = 0; i < cells.size(); ++i) {
		if (i > 0) ss << ",";
		ss << "[" << cells[i].first << "," << cells[i].second << "]";
	}
	ss << "]";
	return ss.str();
}

std::vector<std::pair<int, int>> DatabaseManager::decodeCells(const std::string& jsonStr) const {
	std::vector<std::pair<int, int>> cells;
	if (jsonStr.empty()) return cells;

	size_t pos = 0;
	while (true) {
		size_t start = std::string::npos;
		size_t searchFrom = pos;
		while (searchFrom < jsonStr.size()) {
			size_t candidate = jsonStr.find('[', searchFrom);
			if (candidate == std::string::npos) break;
			if (candidate + 1 < jsonStr.size() &&
				(std::isdigit(jsonStr[candidate + 1]) || jsonStr[candidate + 1] == '-')) {
				start = candidate;
				break;
			}
			searchFrom = candidate + 1;
		}
		if (start == std::string::npos) break;

		size_t comma = jsonStr.find(',', start);
		size_t end = jsonStr.find(']', comma);
		if (comma == std::string::npos || end == std::string::npos) break;

		try {
			int x = std::stoi(jsonStr.substr(start + 1, comma - start - 1));
			int y = std::stoi(jsonStr.substr(comma + 1, end - comma - 1));
			cells.push_back({ x, y });
		}
		catch (...) {}

		pos = end + 1;
	}

	return cells;
}

std::vector<std::pair<int, int>> DatabaseManager::getPatternCells(const std::string& patternName) {
	if (!isConnected()) return {};
	try {
		nanodbc::statement stmt(*conn,
			NANODBC_TEXT("SELECT cells FROM dbo.Patterns WHERE nazwa = ?"));
		stmt.bind(0, patternName.c_str());

		auto result = nanodbc::execute(stmt);
		if (result.next()) {
			return decodeCells(result.get<std::string>(0, std::string{}));
		}
	}
	catch (const std::exception& e) {
		std::cerr << "getPatternCells failed: " << e.what() << std::endl;
	}
	return {};
}

bool DatabaseManager::savePattern(const std::string& patternName,
	const std::vector<std::pair<int, int>>& cells) {
	if (!isConnected()) return false;
	try {
		const std::string cellsJson = encodeCells(cells);
		nanodbc::statement checkStmt(*conn,
			NANODBC_TEXT("SELECT COUNT(*) FROM dbo.Patterns WHERE nazwa = ?"));
		checkStmt.bind(0, patternName.c_str());
		auto checkResult = nanodbc::execute(checkStmt);
		checkResult.next();
		int count = checkResult.get<int>(0, 0);

		if (count > 0) {
			nanodbc::statement stmt(*conn,
				NANODBC_TEXT("UPDATE dbo.Patterns SET cells = ? WHERE nazwa = ?"));
			stmt.bind(0, cellsJson.c_str());
			stmt.bind(1, patternName.c_str());
			nanodbc::execute(stmt);
		}
		else {
			nanodbc::statement stmt(*conn,
				NANODBC_TEXT("INSERT INTO dbo.Patterns (nazwa, cells) VALUES (?, ?)"));
			stmt.bind(0, patternName.c_str());
			stmt.bind(1, cellsJson.c_str());
			nanodbc::execute(stmt);
		}

		std::cout << "Pattern '" << patternName << "' saved." << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "savePattern failed: " << e.what() << std::endl;
		return false;
	}
}

std::vector<std::string> DatabaseManager::getAllPatternNames() {
	if (!isConnected()) return {};
	try {
		auto result = nanodbc::execute(*conn,
			NANODBC_TEXT("SELECT nazwa FROM dbo.Patterns ORDER BY id"));

		std::vector<std::string> names;
		while (result.next()) {
			names.push_back(result.get<std::string>(0, std::string{}));
		}
		return names;
	}
	catch (const std::exception& e) {
		std::cerr << "getAllPatternNames failed: " << e.what() << std::endl;
		return {};
	}
}

bool DatabaseManager::saveBoard(const std::vector<std::pair<int, int>>& cells) {
	if (!isConnected()) return false;
	try {
		const std::string cellsJson = encodeCells(cells);
		nanodbc::statement stmt(*conn,
			NANODBC_TEXT("INSERT INTO dbo.GameBoards (cells) VALUES (?)"));
		stmt.bind(0, cellsJson.c_str());
		nanodbc::execute(stmt);
		std::cout << "Board saved." << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "saveBoard failed: " << e.what() << std::endl;
		return false;
	}
}

std::vector<int> DatabaseManager::getAllBoardIds() {
	if (!isConnected()) return {};
	try {
		auto result = nanodbc::execute(*conn,
			NANODBC_TEXT("SELECT id FROM dbo.GameBoards ORDER BY id DESC"));

		std::vector<int> ids;
		while (result.next()) {
			ids.push_back(result.get<int>(0, 0));
		}
		return ids;
	}
	catch (const std::exception& e) {
		std::cerr << "getAllBoardIds failed: " << e.what() << std::endl;
		return {};
	}
}

std::vector<std::pair<int, int>> DatabaseManager::getBoardCells(int boardId) {
	if (!isConnected()) return {};
	try {
		nanodbc::statement stmt(*conn,
			NANODBC_TEXT("SELECT cells FROM dbo.GameBoards WHERE id = ?"));
		stmt.bind(0, &boardId);

		auto result = nanodbc::execute(stmt);
		if (result.next()) {
			return decodeCells(result.get<std::string>(0, std::string{}));
		}
	}
	catch (const std::exception& e) {
		std::cerr << "getBoardCells failed: " << e.what() << std::endl;
	}
	return {};
}

bool DatabaseManager::deletePattern(const std::string& patternName) {
	if (!isConnected()) return false;
	try {
		nanodbc::statement stmt(*conn,
			NANODBC_TEXT("DELETE FROM dbo.Patterns WHERE nazwa = ?"));
		stmt.bind(0, patternName.c_str());
		nanodbc::execute(stmt);
		std::cout << "Pattern '" << patternName << "' deleted." << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "deletePattern failed: " << e.what() << std::endl;
		return false;
	}
}
