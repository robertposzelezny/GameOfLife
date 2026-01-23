#include "Pattern.h"

void Pattern::apply(Grid& g, int offsetY, int offsetX) const{
    for (const auto& cell : cells) {
        int x = offsetX + cell.first;
        int y = offsetY + cell.second;

        // 1. Sprawdzamy, czy Y (wiersz) mieœci siê w siatce
        if (y >= 0 && y < (int)g.size()) {

            // 2. Sprawdzamy, czy X (kolumna) mieœci siê w tym konkretnym wierszu
            // g[y].size() to bezpieczny sposób na sprawdzenie szerokoœci
            if (x >= 0 && x < (int)g[y].size()) {

                // 3. ZAPIS: Najpierw wiersz (y), potem kolumna (x)
                // To jest kluczowe: g[y][x], a nie g[x][y]!
                g[y][x] = 1;
            }
        }
    }
}