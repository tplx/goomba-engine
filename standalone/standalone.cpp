#include <cstring>
#include <ctype.h>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>
#include <cmath>
#include <algorithm>

constexpr int TABLE_SIZE = 1000000;
constexpr int NODES_SEARCHED = 10000;
constexpr int MATE_VALUE = 30000;

using namespace std;

constexpr int A1 = 91, H1 = 98, A8 = 21, H8 = 28;

char initial_position[120] =  "                     rnbqkbnr  pppppppp  ........  ........  ........  ........  PPPPPPPP  RNBQKBNR                    ";


constexpr int N = -10;
constexpr int E = 1;
constexpr int S = 10;
constexpr int W = -1;

map<char, vector<int> > directions;

typedef struct Entry
{
	int depth = -1, score = -1, gamma = -1;
	pair<int, int> bmove = make_pair(-1,-1);
    Entry() : depth( -1 ), score( -1 ), gamma( -1 ), bmove( make_pair( -1, -1 ) ) {}
	Entry( int sDepth, int sScore, int sGamma, pair<int, int> sBmove ) :
		depth( sDepth ),
		score( sScore ),
		gamma( sGamma ),
		bmove( sBmove )
	{
	}
} Entry;

map<char, vector<int> > pst;


class Position
{

public:
    Position( char * sboard, int score, bool * wc, bool * bc, int ep, int kp );
    ~Position();

    vector<pair<int,int> > genMoves();      Position rotate();        Position move( pair<int, int> sMove );
    int value( pair<int, int> sMove );
        bool operator <( const Position& rhs ) const
    {
        return mScore < rhs.mScore;
    }


public:
                            char mBoard[120];      int mScore;            bool mWc[2];           bool mBc[2];           int mEp;               int mKp;           


};

map<Position, Entry> tp; 
vector<pair<int, int> > Position::genMoves()
{
	vector<pair<int, int> > moves;
		for( int i = 0; i < 120; i++ )
	{
		if( isupper( mBoard[i] ) )
		{
			vector<int> dirs = directions[mBoard[i]];
			for( int d = 0; d < dirs.size(); d++ )
			{
				int di = dirs[d];
				for( int j = (di+i); ; j += di )
				{
					char q = mBoard[j];
					
										if( q == ' ' )
					{
						break;
					}
										if( i == A1 && q == 'K' && mWc[0] )
					{
												moves.push_back( make_pair( j, j-2 ) );
					}
					if( i == H1 && q == 'K' && mWc[1] )
					{
												moves.push_back( make_pair( j, j+2 ) );
					}
										if( isupper( q ) )
					{
						break;
					}
										if( mBoard[i] == 'P' && ( di == N+W || di == N+E ) && q == '.' && ( j != mEp && j != mKp ) )
					{
						break;
					}
					if( mBoard[i] == 'P' && ( di == N || di == 2*N ) && q != '.' )
					{
						break;
					}
					if( mBoard[i] == 'P' && di == 2*N && ( i < A1+N || mBoard[i+N] != '.' ) )
					{
						break;
					}
					
															moves.push_back( make_pair( i, j ) );
					
										if( mBoard[i] == 'P' || mBoard[i] == 'N' || mBoard[i] == 'K' )
					{
						break;
					}
					
										if( islower( q ) )
					{
						break;
					}
				}
			}
		}
	}
	return moves; }

Position::Position( char * board, int score, bool * wc, bool * bc, int ep, int kp )
{
    strcpy( mBoard, board );     mScore = score;
    memcpy( mWc, wc, sizeof( mWc ) );
    memcpy( mBc, bc, sizeof( mBc ) );
    mEp = ep;
    mKp = kp;
}
Position::~Position()
{

}

Position Position::rotate()
{
				char swapBoard[120];
		strcpy( swapBoard, mBoard );
	int len = strlen( swapBoard );
	for( int i = 0; i < len / 2; i++ )
	{
		swapBoard[i] ^= swapBoard[len - i - 1];
		swapBoard[len - i - 1] ^= swapBoard[i];
		swapBoard[i] ^= swapBoard[len - i - 1];
	}
        for( int i = 0; i < len; i++ )
    {
        if( isalpha( swapBoard[i] ) )
        {
            if( islower( swapBoard[i] ) )
            {
                swapBoard[i] = toupper( swapBoard[i] );
            }
            else if( isupper( swapBoard[i] ) )
            {
                swapBoard[i] = tolower( swapBoard[i] );
            }
        }
    }
	return Position( swapBoard, -mScore, mBc, mWc, 119 - mEp, 119 - mKp );
}

Position Position::move( pair<int, int> sMove )
{
	int i = sMove.first;
	int j = sMove.second;
	char p = mBoard[i];
	char q = mBoard[j];
				
		char board[120];
	strcpy( board, mBoard );
	int ep = 0, kp = 0;
	bool wc[2], bc[2];
	memcpy( wc, mWc, sizeof( wc ) );
    memcpy( bc, mBc, sizeof( bc ) );
	int score = mScore + value( sMove );
		board[j] = board[i];
	board[i] = '.';
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
		if( abs( j - i ) == 2 )
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

    return Position( board, score, wc, bc, ep, kp ).rotate();
}

int Position::value( pair<int, int> sMove )
{
    int i = sMove.first;
    int j = sMove.second;
    char p = mBoard[i];
    char q = mBoard[j];

        int score = pst[toupper(p)][j] - pst[toupper(p)][i];

        if( islower( q ) )
    {
        score += pst[toupper( q )][j];
    }
    if( abs( j - mKp ) < 2 )
    {
        score += pst['K'][j];
    }
        if( p == 'K' && abs( i - j ) == 2 )
    {
        score += pst['R'][floor( ( i + j ) / 2 )];
        score -= pst['R'][(j<i) ? A1 : H1];
    }
        if( p == 'P' )
    {
        if( A8 <= j && j <= H8 )
        {
            score += pst['Q'][j] - pst['P'][j];
        }
        if( j == mEp )
        {
            score += pst['P'][j + S];
        }
    }
    return score;
}

// here

int nodes = 0; 
int bound(Position pos, int gamma, int depth)
{
    nodes += 1;

        Entry initEntry = tp[pos];
    if (initEntry.depth != -1 && initEntry.depth >= depth && 
        (initEntry.score < initEntry.gamma && initEntry.score < gamma || 
         initEntry.score >= initEntry.gamma && initEntry.score >= gamma))
    {
        return initEntry.score;
    }

        if (abs(pos.mScore) >= MATE_VALUE)
    {
        return pos.mScore;
    }

        int nullscore = (depth > 0) ? -bound(pos.rotate(), 1 - gamma, depth - 3) : pos.mScore;

        if (nullscore >= gamma)
    {
        return nullscore;
    }

    int best = -3 * MATE_VALUE;
    pair<int, int> bmove;

        vector<pair<int, int>> moves = pos.genMoves();

        sort(moves.begin(), moves.end(), [&](pair<int, int> a, pair<int, int> b) {
        return pos.value(a) > pos.value(b);
    });

        for (pair<int, int> move : moves)
    {
                if (depth <= 0 && pos.value(move) < 150)
        {
            break;
        }

                int score = -bound(pos.move(move), 1 - gamma, depth - 1);

                if (score > best)
        {
            best = score;
            bmove = move;
        }

                if (score >= gamma)
        {
            break;
        }
    }

        if (depth <= 0 && best < nullscore)
    {
        return nullscore;
    }

        if (depth > 0 && best <= -MATE_VALUE && nullscore > -MATE_VALUE)
    {
        best = 0;
    }

        if (initEntry.depth == -1 || depth >= initEntry.depth && best >= gamma)
    {
        tp[pos] = Entry(depth, best, gamma, bmove);
        if (tp.size() > TABLE_SIZE)
        {
            tp.erase(tp.end());
        }
    }

    return best; 
}

pair<pair<int, int>, int> search(Position pos, int maxn = NODES_SEARCHED) {
    nodes = 0;
    int score;
    pair<int, int> best_move;
    for (int depth = 1; depth < 99; depth++)
    {
        int lower = -3 * MATE_VALUE;
        int upper = 3 * MATE_VALUE;
        while (lower < (upper - 3))
        {
            int gamma = static_cast<int>((lower + upper + 1) / 2);
            score = bound(pos, gamma, depth);
            if (score >= gamma)
            {
                lower = score;
                best_move = tp[pos].bmove;             }
            if (score < gamma)
            {
                upper = score;
            }
        }
        if (nodes >= maxn || abs(score) >= MATE_VALUE)
        {
            break;
        }
    }
    return make_pair(best_move, score); }


pair<int, int> parse( string sMove )
{
		if( sMove.length() != 4 )
	{
		return make_pair( 0, 0 );
	}
		int file1 = sMove.at( 0 ) - 'a';
	int rank1 = ( sMove.at( 1 ) - '0' ) - 1;
	int pos1 = A1 + file1 - 10 * rank1;

		int file2 = sMove.at( 2 ) - 'a';
	int rank2 = ( sMove.at( 3 ) - '0' ) - 1;
	int pos2 = A1 + file2 - 10 * rank2;

	return make_pair( pos1, pos2 );
}

string render(int i) {     int rank = (i - A1) / 10;
    int file = (i - A1) % 10;
    char fil = file + 'a';
    return string(1, fil) + to_string(-rank + 1);
}


void printBoard( char * board )
{
        for( int i = 0; i < 120; i++ )
    {
        printf( "%c ", board[i] );
        if( i % 10 == 9 )
        {
            printf( "\n" );
        }
    }
    printf( "\n" );
}

int main()
{
    directions['P'] = { N, 2 * N, N + W, N + E };
    directions['B'] = { N + E, S + E, S + W, N + W };
    directions['N'] = { 2 * N + E, N + 2 * E, S + 2 * E, 2 * S + E, 2 * S + W, S + 2 * W, N + 2 * W, 2 * N + W };
    directions['R'] = { N, E, S, W };
    directions['Q'] = { N, E, S, W, N + E, S + E, S + W, N + W };
    directions['K'] = { N, E, S, W, N + E, S + E, S + W, N + W };

    pst['P'] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 198, 198, 198, 198, 198, 198, 198, 198, 0, 0, 178, 198, 198, 198, 198, 198, 198, 178, 0, 0, 178, 198, 198, 198, 198, 198, 198, 178, 0, 0, 178, 198, 208, 218, 218, 208, 198, 178, 0, 0, 178, 198, 218, 238, 238, 218, 198, 178, 0, 0, 178, 198, 208, 218, 218, 208, 198, 178, 0, 0, 178, 198, 198, 198, 198, 198, 198, 178, 0, 0, 198, 198, 198, 198, 198, 198, 198, 198, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    pst['B'] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 797, 824, 817, 808, 808, 817, 824, 797, 0, 0, 814, 841, 834, 825, 825, 834, 841, 814, 0, 0, 818, 845, 838, 829, 829, 838, 845, 818, 0, 0, 824, 851, 844, 835, 835, 844, 851, 824, 0, 0, 827, 854, 847, 838, 838, 847, 854, 827, 0, 0, 826, 853, 846, 837, 837, 846, 853, 826, 0, 0, 817, 844, 837, 828, 828, 837, 844, 817, 0, 0, 792, 819, 812, 803, 803, 812, 819, 792, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    pst['N'] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 627, 762, 786, 798, 798, 786, 762, 627, 0, 0, 763, 798, 822, 834, 834, 822, 798, 763, 0, 0, 817, 852, 876, 888, 888, 876, 852, 817, 0, 0, 797, 832, 856, 868, 868, 856, 832, 797, 0, 0, 799, 834, 858, 870, 870, 858, 834, 799, 0, 0, 758, 793, 817, 829, 829, 817, 793, 758, 0, 0, 739, 774, 798, 810, 810, 798, 774, 739, 0, 0, 683, 718, 742, 754, 754, 742, 718, 683, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    pst['R'] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1258, 1263, 1268, 1272, 1272, 1268, 1263, 1258, 0, 0, 1258, 1263, 1268, 1272, 1272, 1268, 1263, 1258, 0, 0, 1258, 1263, 1268, 1272, 1272, 1268, 1263, 1258, 0, 0, 1258, 1263, 1268, 1272, 1272, 1268, 1263, 1258, 0, 0, 1258, 1263, 1268, 1272, 1272, 1268, 1263, 1258, 0, 0, 1258, 1263, 1268, 1272, 1272, 1268, 1263, 1258, 0, 0, 1258, 1263, 1268, 1272, 1272, 1268, 1263, 1258, 0, 0, 1258, 1263, 1268, 1272, 1272, 1268, 1263, 1258, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    pst['Q'] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 0, 0, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 0, 0, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 0, 0, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 0, 0, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 0, 0, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 0, 0, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 0, 0, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 2529, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    pst['K'] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60098, 60132, 60073, 60025, 60025, 60073, 60132, 60098, 0, 0, 60119, 60153, 60094, 60046, 60046, 60094, 60153, 60119, 0, 0, 60146, 60180, 60121, 60073, 60073, 60121, 60180, 60146, 0, 0, 60173, 60207, 60148, 60100, 60100, 60148, 60207, 60173, 0, 0, 60196, 60230, 60171, 60123, 60123, 60171, 60230, 60196, 0, 0, 60224, 60258, 60199, 60151, 60151, 60199, 60258, 60224, 0, 0, 60287, 60321, 60262, 60214, 60214, 60262, 60321, 60287, 0, 0, 60298, 60332, 60273, 60225, 60225, 60273, 60332, 60298, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


    bool initBools[] = { true, true };
    Position pos( initial_position, 0, initBools, initBools, 0, 0 );
    while( 1 )
    {
        printBoard( pos.mBoard );
        		pair<int, int> move;
		vector<pair<int, int> > generatedMoves = pos.genMoves();
		while( std::find_if( generatedMoves.begin(), generatedMoves.end(), [&](const pair<int, int>& p) { return p == move; } ) == generatedMoves.end() )
		{
						string input;
            cout << "Input move: ";
			cin >> input;
			move = parse( input );
		}
		pos = pos.move( move );
				printBoard( pos.rotate().mBoard );

        pair<pair<int, int>, int> moveScore = search( pos );
        move = moveScore.first;
        int score = moveScore.second;
        if( score <= -MATE_VALUE )
        {
            printf( "You won" );
            return 0;
        }
        if( score >= MATE_VALUE )
        {
            printf( "You lost" );
            return 0;
        }
		
                pos = pos.move( move );

    }

    return 0;
}