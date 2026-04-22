#include "Pattern.h"
#include "DatabaseManager.h"

Pattern::Pattern(const std::string& patternName)
    : patternName(patternName) {
    if (!patternName.empty()) {
        loadFromDatabase(patternName);
    }
}

void Pattern::apply(Grid& g, int offsetY, int offsetX) const {
    for (const auto& cell : cells) {
        int x = offsetX + cell.first;
        int y = offsetY + cell.second;
        if (y >= 0 && y < (int)g.size()) {
            if (x >= 0 && x < (int)g[y].size()) {
                g[y][x] = 1;
            }
        }
    }
}

bool Pattern::loadFromDatabase(const std::string& name) {
    patternName = name;
    DatabaseManager& db = DatabaseManager::getInstance();

    if (!db.isConnected()) {
        return false;
    }

    cells = db.getPatternCells(name);
    return !cells.empty() || name.empty();
}

bool Pattern::saveToDatabase() const {
    if (patternName.empty()) {
        return false;
    }

    DatabaseManager& db = DatabaseManager::getInstance();
    if (!db.isConnected()) {
        return false;
    }

    return db.savePattern(patternName, cells);
}

bool Pattern::deleteFromDatabase() const {
    if (patternName.empty()) {
        return false;
    }

    DatabaseManager& db = DatabaseManager::getInstance();
    if (!db.isConnected()) {
        return false;
    }

    return db.deletePattern(patternName);
}

const std::vector<std::pair<int, int>>& Pattern::getCells() const {
    return cells;
}

void Pattern::setPatternName(const std::string& name) {
    patternName = name;
}

const std::string& Pattern::getPatternName() const {
    return patternName;
}
