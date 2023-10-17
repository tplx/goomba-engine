#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <utility>

class Entry {
public:
    Entry();
    Entry(int depth, int score, int gamma, std::pair<int, int> bmove);
    int getDepth() const;
    int getScore() const;
    int getGamma() const;
    std::pair<int, int> getBMove() const;

private:
    int depth;
    int score;
    int gamma;
    std::pair<int, int> bmove;
};

#endif // ENTRY_HPP
