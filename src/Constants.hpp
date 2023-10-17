#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <vector>
#include <map>
#include <array>

static constexpr int TABLE_SIZE = 100000;
static constexpr int NODES_SEARCHED = 10000;
static constexpr int MATE_VALUE = 30000;
static constexpr int FLAG_EXACT = 1;
static constexpr int A1 = 91;
static constexpr int H1 = 98;
static constexpr int A8 = 21;
static constexpr int H8 = 28;
static constexpr char initial[120] = "                     rnbqkbnr  pppppppp  ........  ........  ........  ........  PPPPPPPP  RNBQKBNR                    ";
static const std::map<char, std::vector<int>> directions;
static const std::map<char, std::vector<int>> pst;
static constexpr std::array<bool, 2> initBools = { true, true }; // using std::array instead of std::vector
const int N = -10;
const int E = 1;
const int S = 10;
const int W = -1;

#endif // CONSTANTS_HPP
