#ifndef RENDER_HPP
#define RENDER_HPP

#include <string>

class Render {
public:
    static void printBoard(const Position& pos);
    static std::string render(int i);
};

#endif // RENDER_HPP
