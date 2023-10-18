#include "Constants.hpp"
#include "Position.hpp"
#include "Search.hpp"
#include "Render.hpp"
#include <iostream>
#include <algorithm>

int main() {
    std::vector<bool> initBools = { true, true };
    Position pos(initial, 0, initBools, initBools, 0, 0);
    Render render;
    Search search;

    while (true) {
        render.printBoard(pos);

        std::pair<int, int> move;
        std::vector<std::pair<int, int>> generatedMoves = pos.genMoves();

        if (pos.getTurn()) {
            std::string input;
            std::cout << "Your move: ";
            std::getline(std::cin, input);
            move = search.parse(input);
        } else {
            std::cout << "Thinking...\n";
            move = search.search(pos).first;
        }

        if (std::find(generatedMoves.begin(), generatedMoves.end(), move) != generatedMoves.end()) {
            pos = pos.move(move);
        } else {
            std::cout << "Invalid move\n";
            continue;
        }

        render.printBoard(pos.rotate());

        std::pair<std::pair<int, int>, int> moveScore = search.search(pos);
        move = moveScore.first;
        int score = moveScore.second;
        if (score <= -MATE_VALUE) {
            std::cout << "You won\n";
            return 0;
        }
        if (score >= MATE_VALUE) {
            std::cout << "You lost\n";
            return 0;
        }

        pos = pos.move(move);
    }

    return 0;
}
