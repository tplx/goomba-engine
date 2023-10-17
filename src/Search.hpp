#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <utility>
#include <string>
#include "Position.hpp"

class Search {
public:
    std::pair<std::pair<int, int>, int> search(Position pos, int maxn = NODES_SEARCHED);
    std::pair<int, int> parse(const std::string& sMove);

private:
    std::unordered_map<Position, Entry, std::hash<Position>> tp;
    std::unordered_map<Position, std::pair<int, int>, std::hash<Position>> best_moves;
    int nodes = 0;

    int bound(Position pos, int gamma, int depth);
};

#endif // SEARCH_HPP
