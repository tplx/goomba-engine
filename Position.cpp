#include "Position.hpp"
#include "Constants.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

Position::Position(const char board[120], int score, std::vector<bool> wCastle, std::vector<bool> bCastle, int ep, int kp)
        : mScore(score), mCastleWhite(wCastle), mCastleBlack(bCastle), mEpSquare(ep), mKingPos(kp) {
        std::copy(board, board + 120, mBoard.begin());
}

// Rotate the board
Position Position::rotate() const
{
    std::array<char, 120> newBoard;
    std::reverse_copy(mBoard.begin(), mBoard.end(), newBoard.begin());
    return Position(newBoard.data(), mScore, mCastleBlack, mCastleWhite, 119 - mEpSquare, 119 - mKingPos);
}

// Generate possible moves
std::vector<std::pair<int, int>> Position::genMoves() const {
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < 120; i++) {
        char piece = toupper(mBoard[i]);
        if (directions.count(piece) == 0) continue;
        for (int j = 0; j < directions.at(piece).size(); j++) {
            for (int n = i;;) {
                n += directions.at(piece).at(j);
                if (n < 0 || n >= mBoard.size()) break;
                if (mBoard[n] == '.')
                    moves.push_back(std::make_pair(i, n));
                else {
                    if (std::islower(mBoard[i]) != std::islower(mBoard[n]))
                        moves.push_back(std::make_pair(i, n));
                    break;
                }
                if (toupper(mBoard[i]) == 'P' || toupper(mBoard[i]) == 'N')
                    break;
            }
        }
    }
    return moves;
}

Position Position::move(const std::pair<int, int> &move) const {
    int i = move.first;
    int j = move.second;
        char p = mBoard[i];
    char q = mBoard[j];

    std::array<char, 120> board = mBoard;
    int ep = 0, kp = 0;

    // Copy the castling rights
    std::vector<bool> wc = mCastleWhite;
    std::vector<bool> bc = mCastleBlack;

    int score = mScore + value( move );
    // Actual move
    board[j] = board[i];
    board[i] = '.';
    // Castling rights
    if( i == A1 )
    {
        wc[0] = false;
    }
    if( i == H1 )
    {
        wc[1] = false;
    }
    if( j == A8 )
    {
        bc[1] = false;
    }
    if( j == H8 )
    {
        bc[0] = false;
    }
    if( p == 'K' )
    {
        wc[0] = false; wc[1] = false;
        if( std::abs( j - i ) == 2 )
        {
            kp = static_cast<int>( (i+j) / 2 );
            board[ (j<i) ? A1 : H1 ] = '.';
            board[kp] = 'R';
        }
    }
    if( p == 'P' )
    {
        if( A8 <= j && j <= H8 )
        {
            board[j] = 'Q';
        }
        if( j - i == 2 * N )
        {
            ep = i + N;
        }
        if( ( j - i == N + W || j - i == N + E ) && q == '.' )
        {
            board[j + S] = '.';
        }
    }

    return Position( board.data(), score, wc, bc, ep, kp ).rotate();
}

int Position::value(const std::pair<int, int> &move) const {
    int i = move.first;
    int j = move.second;
    char p = mBoard[i];
    char q = mBoard[j];
    int score = 0;
    // Actual move
    try {
        if (i >= 0 && i < pst.at(std::toupper(p)).size() && j >= 0 && j < pst.at(std::toupper(p)).size()) {
            score = pst.at(std::toupper(p)).at(j) - pst.at(std::toupper(p)).at(i);
        }
    } catch(const std::out_of_range& e) {
        std::cerr << "Out of range error: " << e.what() << '\n';
        std::cerr << "p: " << p << ", i: " << i << ", j: " << j << '\n';
    }

    // Capture
    if( std::islower( q ) )
    {
        score += pst.at(std::toupper( q )).at(j);
    }
    if( std::abs( j - mKingPos ) < 2 )
    {
        score += pst.at('K').at(j);
    }
    //Castling
    if( p == 'K' && abs( i - j ) == 2 )
    {
        score += pst.at('R').at(floor( ( i + j ) / 2 ));
        score -= pst.at('R').at((j<i) ? A1 : H1);
    }
    // More special pawn movement
    if( p == 'P' )
    {
        if( A8 <= j && j <= H8 )
        {
            score += pst.at('Q').at(j) - pst.at('P').at(j);
        }
        if( j == mEpSquare )
        {
            score += pst.at('P').at(j + S);
        }
    }
    return score;
}
