#include "DatabaseManager.h"
#include <iostream>
#include <sstream>
#include <cstring>

namespace {
	void printOdbcError(SQLSMALLINT handleType, SQLHANDLE handle, const char* context) {
		SQLCHAR sqlState[6] = {};
		SQLINTEGER nativeError = 0;
		SQLCHAR messageText[512] = {};
		SQLSMALLINT textLength = 0;
		SQLRETURN ret = SQLGetDiagRec(handleType, handle, 1, sqlState, &nativeError, messageText, sizeof(messageText), &textLength);
		if (SQL_SUCCEEDED(ret)) {
			std::cerr << context << " | SQLSTATE=" << sqlState << " NativeError=" << nativeError << " Message=" << messageText << std::endl;
		}
		else {
			std::cerr << context << std::endl;
		}
	}
}

DatabaseManager& DatabaseManager::getInstance() {
	static DatabaseManager instance;
	return instance;
}

DatabaseManager::DatabaseManager() 
	: env(NULL), dbc(NULL), connected(false) {
}

DatabaseManager::~DatabaseManager() {
	disconnect();
}

bool DatabaseManager::connect() {
	if (connected) return true;

	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to allocate environment handle" << std::endl;
		return false;
	}
	ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to set ODBC version" << std::endl;
		return false;
	}
	ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to allocate connection handle" << std::endl;
		return false;
	}
	SQLCHAR connStr[] =
		"DRIVER={SQL Server};SERVER=DESKTOP-TQN13PE;DATABASE=GameOfLifeDB;Trusted_Connection=yes;";

	SQLCHAR outstr[1024];
	SQLSMALLINT outstrlen;

	ret = SQLDriverConnect(
		dbc,
		NULL,
		connStr,
		SQL_NTS,
		outstr,
		sizeof(outstr),
		&outstrlen,
		SQL_DRIVER_COMPLETE
	);

	if (SQL_SUCCEEDED(ret)) {
		std::cout << "Connected to database!" << std::endl;
		connected = true;
		return true;
	}
	else {
		printOdbcError(SQL_HANDLE_DBC, dbc, "Connection failed!");
		connected = false;
		return false;
	}
}

void DatabaseManager::disconnect() {
	if (dbc != NULL) {
		SQLDisconnect(dbc);
		SQLFreeHandle(SQL_HANDLE_DBC, dbc);
		dbc = NULL;
	}
	if (env != NULL) {
		SQLFreeHandle(SQL_HANDLE_ENV, env);
		env = NULL;
	}
	connected = false;
}

bool DatabaseManager::isConnected() const {
	return connected;
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

	std::string content = jsonStr;
	if (content.front() == '[') content = content.substr(1);
	if (content.back() == ']') content = content.substr(0, content.length() - 1);

	std::stringstream ss(content);
	std::string token;

	while (std::getline(ss, token, '[')) {
		if (token.empty()) continue;

		size_t commaPos = token.find(',');
		if (commaPos != std::string::npos) {
			try {
				int x = std::stoi(token.substr(0, commaPos));
				int y = std::stoi(token.substr(commaPos + 1, token.find(']')));
				cells.push_back({x, y});
			}
			catch (...) {
			}
		}
	}

	return cells;
}

std::vector<std::pair<int, int>> DatabaseManager::getPatternCells(const std::string& patternName) {
	std::vector<std::pair<int, int>> result;

	if (!isConnected()) {
		std::cerr << "Database not connected" << std::endl;
		return result;
	}

	SQLHSTMT stmt;
	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to allocate statement handle" << std::endl;
		return result;
	}

	std::string query = "SELECT cells FROM dbo.Patterns WHERE nazwa = ?";

	ret = SQLPrepare(stmt, (SQLCHAR*)query.c_str(), SQL_NTS);
	if (!SQL_SUCCEEDED(ret)) {
		printOdbcError(SQL_HANDLE_STMT, stmt, "Failed to prepare statement");
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return result;
	}

	SQLCHAR paramStr[256];
	strcpy_s((char*)paramStr, sizeof(paramStr), patternName.c_str());
	ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 
						  patternName.length(), 0, paramStr, patternName.length(), NULL);
	if (!SQL_SUCCEEDED(ret)) {
		printOdbcError(SQL_HANDLE_STMT, stmt, "Failed to bind parameter");
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return result;
	}

	ret = SQLExecute(stmt);
	if (!SQL_SUCCEEDED(ret)) {
		printOdbcError(SQL_HANDLE_STMT, stmt, "Failed to execute query");
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return result;
	}
	SQLCHAR cellsBuffer[4096];
	SQLLEN cellsLen;

	ret = SQLFetch(stmt);
	if (SQL_SUCCEEDED(ret)) {
		SQLGetData(stmt, 1, SQL_C_CHAR, cellsBuffer, sizeof(cellsBuffer), &cellsLen);
		if (cellsLen > 0) {
			std::string cellsJson((char*)cellsBuffer, cellsLen);
			result = decodeCells(cellsJson);
		}
	}

	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return result;
}

bool DatabaseManager::savePattern(const std::string& patternName, 
								 const std::vector<std::pair<int, int>>& cells) {
	if (!isConnected()) {
		std::cerr << "Database not connected" << std::endl;
		return false;
	}

	SQLHSTMT stmt;
	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to allocate statement handle" << std::endl;
		return false;
	}
	std::string cellsJson = encodeCells(cells);
	std::string checkQuery = "SELECT COUNT(*) FROM dbo.Patterns WHERE nazwa = ?";
	ret = SQLPrepare(stmt, (SQLCHAR*)checkQuery.c_str(), SQL_NTS);

	SQLCHAR paramStr[256];
	strcpy_s((char*)paramStr, sizeof(paramStr), patternName.c_str());
	SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
					patternName.length(), 0, paramStr, patternName.length(), NULL);

	SQLExecute(stmt);

	SQLINTEGER count = 0;
	SQLFetch(stmt);
	SQLGetData(stmt, 1, SQL_C_LONG, &count, 0, NULL);

	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to allocate statement handle" << std::endl;
		return false;
	}

	std::string query;
	if (count > 0) {
		query = "UPDATE dbo.Patterns SET cells = ? WHERE nazwa = ?";
	}
	else {
		query = "INSERT INTO dbo.Patterns (nazwa, cells) VALUES (?, ?)";
	}

	ret = SQLPrepare(stmt, (SQLCHAR*)query.c_str(), SQL_NTS);
	if (!SQL_SUCCEEDED(ret)) {
		printOdbcError(SQL_HANDLE_STMT, stmt, "Failed to prepare statement");
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return false;
	}

	SQLCHAR cellsStr[4096];
	strcpy_s((char*)cellsStr, sizeof(cellsStr), cellsJson.c_str());
	strcpy_s((char*)paramStr, sizeof(paramStr), patternName.c_str());

	if (count > 0) {
		SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR,
						cellsJson.length(), 0, cellsStr, cellsJson.length(), NULL);
		SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
						patternName.length(), 0, paramStr, patternName.length(), NULL);
	}
	else {
		SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
						patternName.length(), 0, paramStr, patternName.length(), NULL);
		SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR,
						cellsJson.length(), 0, cellsStr, cellsJson.length(), NULL);
	}

	ret = SQLExecute(stmt);
	bool success = SQL_SUCCEEDED(ret);

	if (success) {
		std::cout << "Pattern '" << patternName << "' saved to database" << std::endl;
	}
	else {
		printOdbcError(SQL_HANDLE_STMT, stmt, "Failed to save pattern");
	}

	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return success;
}

std::vector<std::string> DatabaseManager::getAllPatternNames() {
	std::vector<std::string> result;

	if (!isConnected()) {
		std::cerr << "Database not connected" << std::endl;
		return result;
	}

	SQLHSTMT stmt;
	SQLRETURN ret;

	ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to allocate statement handle" << std::endl;
		return result;
	}

	std::string query = "SELECT nazwa FROM dbo.Patterns ORDER BY id";
	ret = SQLExecDirect(stmt, (SQLCHAR*)query.c_str(), SQL_NTS);

	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to execute query" << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return result;
	}

	SQLCHAR nameBuffer[256];
	SQLLEN nameLen;

	while (SQLFetch(stmt) == SQL_SUCCESS) {
		SQLGetData(stmt, 1, SQL_C_CHAR, nameBuffer, sizeof(nameBuffer), &nameLen);
		if (nameLen > 0) {
			result.push_back(std::string((char*)nameBuffer, nameLen));
		}
	}

	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return result;
}

bool DatabaseManager::saveBoard(const std::vector<std::pair<int, int>>& cells) {
	if (!isConnected()) {
		std::cerr << "Database not connected" << std::endl;
		return false;
	}

	SQLHSTMT stmt;
	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to allocate statement handle" << std::endl;
		return false;
	}

	const std::string cellsJson = encodeCells(cells);
	const std::string query = "INSERT INTO dbo.GameBoards (cells) VALUES (?)";
	ret = SQLPrepare(stmt, (SQLCHAR*)query.c_str(), SQL_NTS);
	if (!SQL_SUCCEEDED(ret)) {
		printOdbcError(SQL_HANDLE_STMT, stmt, "Failed to prepare board insert");
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return false;
	}

	SQLLEN cellsLen = SQL_NTS;
	ret = SQLBindParameter(
		stmt,
		1,
		SQL_PARAM_INPUT,
		SQL_C_CHAR,
		SQL_LONGVARCHAR,
		static_cast<SQLULEN>(cellsJson.size()),
		0,
		(SQLPOINTER)cellsJson.c_str(),
		0,
		&cellsLen
	);
	if (!SQL_SUCCEEDED(ret)) {
		printOdbcError(SQL_HANDLE_STMT, stmt, "Failed to bind board cells");
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return false;
	}

	ret = SQLExecute(stmt);
	const bool success = SQL_SUCCEEDED(ret);
	if (success) {
		std::cout << "Board saved to database" << std::endl;
	}
	else {
		printOdbcError(SQL_HANDLE_STMT, stmt, "Failed to save board");
	}

	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return success;
}

bool DatabaseManager::deletePattern(const std::string& patternName) {
	if (!isConnected()) {
		std::cerr << "Database not connected" << std::endl;
		return false;
	}

	SQLHSTMT stmt;
	SQLRETURN ret;

	ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to allocate statement handle" << std::endl;
		return false;
	}

	std::string query = "DELETE FROM dbo.Patterns WHERE nazwa = ?";
	ret = SQLPrepare(stmt, (SQLCHAR*)query.c_str(), SQL_NTS);
	if (!SQL_SUCCEEDED(ret)) {
		printOdbcError(SQL_HANDLE_STMT, stmt, "Failed to prepare statement");
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return false;
	}

	SQLCHAR paramStr[256];
	strcpy_s((char*)paramStr, sizeof(paramStr), patternName.c_str());
	SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
					patternName.length(), 0, paramStr, patternName.length(), NULL);

	ret = SQLExecute(stmt);
	bool success = SQL_SUCCEEDED(ret);

	if (success) {
		std::cout << "Pattern '" << patternName << "' deleted from database" << std::endl;
	}
	else {
		printOdbcError(SQL_HANDLE_STMT, stmt, "Failed to delete pattern");
	}

	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return success;
}
