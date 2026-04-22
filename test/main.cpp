#include "Game.h"
#include "DatabaseManager.h"
#include "BuiltInPatterns.h"
#include <windows.h>
#include <sqlext.h>
#include <iostream>

int main() {
	// Initialize database connection
	DatabaseManager& db = DatabaseManager::getInstance();

	if (!db.connect()) {
		std::cerr << "Failed to connect to database!" << std::endl;
		return 1;
	}

	if (!seedBuiltInPatterns(db)) {
		std::cerr << "Failed to seed one or more built-in patterns." << std::endl;
	}

	// Run the game
	Game game(60);
	game.runGame();

	// Disconnect from database
	db.disconnect();

	return 0;
}
