#ifndef CUSTY_CREW_H
#define CUSTY_CREW_H
#include "chessPlayer.h"
#include"ENUM.h"

class Custy_Crew : public chessPlayer {
private:
    int movesMade = 0;
    double totalTime = 0;
public:
    Custy_Crew(Color playerColor = White);
    void decideMove(gameState* state, action* Move, int maxDepth);
    
    int countEvals = 0;
    void MiniMaxSearch(gameState state, action* bestMove, int depth);
    double Mini(gameState state, action Move,action* bestMove, double alpha, double beta, int depth);
    double Max(gameState state, action Move,action* bestMove, double alpha, double beta, int depth);

    //move ordering function
    
    void orderMoves(gameState state, action stateActions[], int moves);

    
    //evaluation function
    double evaluateState(gameState state);
    double evaluateMove(gameState state, action Move);
        
    
};


#endif // CUSTY_CREW_H