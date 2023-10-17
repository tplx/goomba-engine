#include "Render.hpp"
#include "Position.hpp"
#include "Constants.hpp"
#include <iostream>

void Render::printBoard(const Position& pos) {
    const std::array<char, 120>& board = pos.getBoard();
    for(int i = 0; i < 120; i++) {
        std::cout << board[i] << " ";
        if(i % 10 == 9) {
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}

std::string Render::render(int i) {
    int rank = (i - A1) / 10;
    int file = (i - A1) % 10;
    char fil = file + 'a';
    return std::string(1, fil) + std::to_string(-rank + 1);
}
