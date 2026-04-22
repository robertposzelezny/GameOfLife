#pragma once
#include <string>
#include <utility>
#include <vector>

class DatabaseManager;

using PatternCells = std::vector<std::pair<int, int>>;
using NamedPattern = std::pair<std::string, PatternCells>;

const std::vector<NamedPattern>& getBuiltInPatterns();
bool seedBuiltInPatterns(DatabaseManager& db);
