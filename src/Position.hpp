#ifndef POSITION_HPP
#define POSITION_HPP

#include <vector>
#include <utility>
#include "Constants.hpp"

class Position {
public:
    Position(const char board[120], int score, std::vector<bool> wCastle, std::vector<bool> bCastle, int ep, int kp);
    Position rotate() const;
    std::vector<std::pair<int, int>> genMoves() const;
    Position move(const std::pair<int, int> &move) const;
    int value(const std::pair<int, int> &move) const;

    int getScore() const {
        return mScore;
    }

    const std::array<char, 120>& getBoard() const {
        return mBoard;
    }

private:
    std::array<char, 120> mBoard;
    int mScore;
    std::vector<bool> mCastleWhite;
    std::vector<bool> mCastleBlack;
    int mEpSquare;
    int mKingPos;
};

#endif // POSITION_HPP
