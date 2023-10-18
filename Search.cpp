#include "Search.hpp"
#include "Position.hpp"
#include "Constants.hpp"
#include "Entry.hpp"
#include <cmath>
#include <utility>
#include <algorithm>
#include <stdexcept>

// Maximum depth for the search function
static constexpr int MAX_DEPTH = 99;

// Minimum value for a move in the bound function
static constexpr int MIN_MOVE_VALUE = 150;

int Search::bound(Position pos, int gamma, int depth) {
    nodes += 1;

    Entry initEntry;
    auto tpIter = tp.find(pos);
    if (tpIter != tp.end()) {
        initEntry = tpIter->second;
        if (initEntry.getDepth() != -1 && initEntry.getDepth() >= depth && 
            (initEntry.getScore() < gamma || initEntry.getScore() >= gamma)) {
            return initEntry.getScore();
        }
    }

    if (std::abs(pos.getScore()) >= MATE_VALUE) {
        return pos.getScore();
    }

    int nullscore = (depth > 0) ? -bound(pos.rotate(), 1 - gamma, depth - 3) : pos.getScore();

    if (nullscore >= gamma) {
        return nullscore;
    }

    int best = -3 * MATE_VALUE;
    std::pair<int, int> bmove;

    std::vector<std::pair<int, int>> moves = pos.genMoves();

    std::sort(moves.begin(), moves.end(), [&](std::pair<int, int> a, std::pair<int, int> b) {
        return pos.value(a) > pos.value(b);
    });

    for (std::pair<int, int> move : moves) {
        if (depth <= 0 && pos.value(move) < MIN_MOVE_VALUE) {
            break;
        }

        int score = -bound(pos.move(move), 1 - gamma, depth - 1);

        if (score > best) {
            best = score;
            bmove = move;
        }

        if (score >= gamma) {
            break;
        }
    }

    if (depth <= 0 && best < nullscore) {
        return nullscore;
    }

    if (depth > 0 && best <= -MATE_VALUE && nullscore > -MATE_VALUE) {
        best = 0;
    }

    if (initEntry.getDepth() == -1 || depth >= initEntry.getDepth() && best >= gamma) {
       tp[pos] = Entry(depth, best, gamma, bmove);
        best_moves[pos] = bmove;
        if (tp.size() > TABLE_SIZE) {
            tp.erase(tp.begin());
        }
    }

    return best; 
}

std::pair<std::pair<int, int>, int> Search::search(Position pos, int maxn) {
    int score;
    std::pair<int, int> best_move;
    for (int depth = 1; depth < MAX_DEPTH; depth++) {
        int lower = -3 * MATE_VALUE;
        int upper = 3 * MATE_VALUE;
        while (lower < (upper - 3)) {
            int gamma = static_cast<int>((lower + upper + 1) / 2);
            score = bound(pos, gamma, depth);
            if (score >= gamma) {
                lower = score;
                auto bestMovesIter = best_moves.find(pos);
                if (bestMovesIter != best_moves.end()) {
                    best_move = bestMovesIter->second;
                }
            }
            if (score < gamma) {
                upper = score;
            }
        }
        if (nodes >= maxn || std::abs(score) >= MATE_VALUE) {
            break;
        }
    }
    return std::make_pair(best_move, score); 
}

std::pair<int, int> Search::parse(const std::string& sMove) {
    if (sMove.length() != 4) {
        throw std::invalid_argument("Invalid move string. Expected string of length 4.");
    }
    int file1 = sMove.at(0) - 'a';
    int rank1 = (sMove.at(1) - '0') - 1;
    int pos1 = A1 + file1 - 10 * rank1;

    int file2 = sMove.at(2) - 'a';
    int rank2 = (sMove.at(3) - '0') - 1;
    int pos2 = A1 + file2 - 10 * rank2;

    return std::make_pair(pos1, pos2);
}