#ifndef CUSTY_CREW_H
#define CUSTY_CREW_H
#include "chessPlayer.h"
#include"ENUM.h"

class Custy_Crew : public chessPlayer {
public:
    Custy_Crew(Color playerColor = White);
    void decideMove(gameState* state, action* Move, int maxDepth);
    
    int countEvals = 0;
    void MiniMaxSearch(gameState state, action* Move, int depth);
    double Mini(gameState state, action* bestMove, double alpha, double beta, int depth);
    double Max(gameState state, action* bestMove, double alpha, double beta, int depth);


    //evaluation function
    double evaluateState(gameState state);

    //Peice-Square Tables
    double KingPST[8][8] = {
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-3, -4, -4, -5, -5, -4, -4, -3},
        {-2, -3, -3, -4, -4, -3, -3, -2},
        {-1, -2, -2, -2, -2, -2, -2, -1},
        { 2,  2,  0,  0,  0,  0,  2,  2},
        { 2,  3,  1,  0,  0,  1,  3,  2}
    };
    double QueenPST[8][8] = {
        {  -2, - 1,  -1, -0.5, -0.5,  -1, -1,   -2},
        {  -1,   0,   0,    0,    0,   0,  0,   -1},
        {  -1,   0, 0.5,  0.5,  0.5, 0.5,  0,   -1},
        {-0.5,   0, 0.5,  0.5,  0.5, 0.5,  0, -0.5},
        {   0,   0, 0.5,  0.5,  0.5, 0.5,  0, -0.5},
        {  -1, 0.5, 0.5,  0.5,  0.5, 0.5,  0,   -1},
        {  -1,   0, 0.5,    0,    0,   0,  0,   -1},
        {  -2,  -1,  -1, -0.5, -0.5,  -1, -1,   -2}
    };
    double RookPST[8][8] = {
       {   0, 0, 0,   0,   0, 0, 0,    0},
       { 0.5, 1, 1,   1,   1, 1, 1,  0.5},
       {-0.5, 0, 0,   0,   0, 0, 0, -0.5},
       {-0.5, 0, 0,   0,   0, 0, 0, -0.5},
       {-0.5, 0, 0,   0,   0, 0, 0, -0.5},
       {-0.5, 0, 0,   0,   0, 0, 0, -0.5},
       {-0.5, 0, 0,   0,   0, 0, 0, -0.5},
       {   0, 0, 0, 0.5, 0.5, 0, 0,    0}
    };
    //https://www.freecodecamp.org/news/simple-chess-ai-step-by-step-1d55a9266977/ gohere and copy paste the remaining for bishop knight and pawn these are garbage values
    double BishopPST[8][8] = {
        {-2,  -1,  -1, -1, -1,  -1,  -1, -2},
        {-1,   0,   0,  0,  0,   0,   0, -1},
        {-1,   0, 0.5,  1,  1, 0.5,   0, -1},
        {-1, 0.5, 0.5,  1,  1, 0.5, 0.5, -1},
        {-1,   0,   1,  1,  1,   1,   0, -1},
        {-1,   1,   1,  1,  1,   1,   1, -1},
        {-1, 0.5,   0,  0,  0,   0, 0.5, -1},
        {-2,  -1,  -1, -1, -1,  -1,  -1, -2}
    };
    double KnightPST[8][8] = {
        {-5,  -4,  -3,  -3,  -3,  -3,  -4, -5},
        {-4,  -2,   0,   0,   0,   0,  -2, -4},
        {-3,   0,   1, 1.5, 1.5,   1,   0, -3},
        {-3, 0.5, 1.5,   2,   2, 1.5, 0.5, -3},
        {-3,   0, 1.5,   2,   2, 1.5,   0, -3},
        {-3, 0.5,   1, 1.5, 1.5,   1,   5, -3},
        {-4,  -2,   0, 0.5, 0.5,   0,  -2, -4},
        {-5,  -4,  -3,  -3,  -3,  -3,  -4, -5}
    };
    double PawnPST[8][8] = {
        {  0,    0,  0,   0,   0,  0,    0,   0},
        {  5,    5,  5,   5,   5,  5,    5,   5},
        {  1,    1,  2,   3,   3,  2,    1,   1},
        {0.5,  0.5,  1, 2.5, 2.5,  1,  0.5, 0.5},
        {  0,    0,  0,   2,   2,  0,    0,   0},
        {0.5, -0.5, -1,   0,   0, -1, -0.5, 0.5},
        {0.5,    1,  1,  -2,  -2,  1,    1, 0.5},
        {  0,    0,  0,   0,   0,  0,    0,   0}
    };
};


#endif // CUSTY_CREW_H