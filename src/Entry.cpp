#include "Entry.hpp"

Entry::Entry() : depth(-1), score(-1), gamma(-1), bmove(std::make_pair(-1, -1)) {}

Entry::Entry(int depth, int score, int gamma, std::pair<int, int> bmove)
    : depth(depth), score(score), gamma(gamma), bmove(bmove) {}

int Entry::getDepth() const {
    return depth;
}

int Entry::getScore() const {
    return score;
}

int Entry::getGamma() const {
    return gamma;
}

std::pair<int, int> Entry::getBMove() const {
    return bmove;
}
