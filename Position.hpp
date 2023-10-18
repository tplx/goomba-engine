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

    bool getTurn() const {
        return mFullMoves % 2 == 0;
    }

    // hashing
    friend struct std::hash<Position>;
    
    bool operator==(const Position& other) const {
        return mBoard == other.mBoard && mScore == other.mScore;
    }
private:
    std::array<char, 120> mBoard;
    int mScore;
    std::vector<bool> mCastleWhite;
    std::vector<bool> mCastleBlack;
    int mEpSquare;
    int mKingPos;
    int mFullMoves = 0; // keep track of the number of full moves
};

// std::unordered_map tp, best_moves from Search.hpp require Position to be hashable - this is a hash function that does that
template <>
struct std::hash<Position> {
    size_t operator()(const Position& pos) const {
            size_t hash = 0;
            for (const auto& item : pos.getBoard()) {
                hash ^= std::hash<char>{}(item) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            hash ^= std::hash<int>{}(pos.getScore()) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
    }
};

#endif // POSITION_HPP
