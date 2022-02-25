#include "..\include\Custy_Crew.h"
#include<iostream>
#include "..\include\\ENUM.h"
#include<cstdlib>

using namespace std;

Custy_Crew::Custy_Crew(Color playerColor) :chessPlayer("Auto Player Name", playerColor) {


}


void Custy_Crew::MiniMaxSearch(gameState state, action* bestMove, int depth) {
    action Move;
    Move.fromCol = Move.fromRow = Move.toCol = Move.toRow = 0;
    Custy_Crew::Max(state, Move, bestMove, INT_MIN, INT_MAX, depth);
    return;
}

double Custy_Crew::Mini(gameState state, action Move,action* bestMove, double alpha, double beta, int depth) {
    state.applyMove(Move);
    if (state.Actions.getActionCount() == 0) {
        //do stalemates, white loss and black loss constitute no evaluation seedha inf or -inf eval?
        if (state.kingUnderThreat(White))
            return -100;
        else if (state.kingUnderThreat(Black))
            return 100;
        else
            return -50;
    }

    else if (depth == 0) {
        return Custy_Crew::evaluateState(state);
    }
    else {
        action minMove;
        minMove.fromRow = minMove.fromCol = minMove.toRow = minMove.toCol = 0;
        int moves = state.Actions.getActionCount();      
        for (int i = 0; i < moves; i++) {
            action move;
            state.Actions.getAction(i, &move);
            double eval = Custy_Crew::Max(state,move, bestMove,alpha, beta, depth - 1);
            if (eval < beta) {
                beta = eval;
                //minMove = move; 
                minMove.fromCol = move.fromCol;
                minMove.fromRow = move.fromRow;
                minMove.toCol = move.toCol;
                minMove.toRow = move.toRow;
                if (eval <= alpha) {
                    break;
                }
            }
           
        }
        bestMove->fromCol = minMove.fromCol;
        bestMove->fromRow = minMove.fromRow;
        bestMove->toCol = minMove.toCol;
        bestMove->toRow = minMove.toRow;
        return beta;
    }
}

double Custy_Crew::Max(gameState state, action Move, action* bestMove, double alpha, double beta, int depth) {
    if (!(Move.fromCol == Move.fromRow == Move.toCol == Move.toRow)) {
        state.applyMove(Move);
    }
    if (state.Actions.getActionCount() == 0) {
        //do stalemates, white loss and black loss constitute no evaluation seedha inf or -inf eval?
        if (state.kingUnderThreat(White))
            return -100;
        else if (state.kingUnderThreat(Black))
            return 100;
        else
            return -50;
    }
    
    else if (depth == 0) {
        return Custy_Crew::evaluateState(state);
    }
    else {
        action maxMove;
        maxMove.fromRow = maxMove.fromCol = maxMove.toRow = maxMove.toCol = 0;
        int moves = state.Actions.getActionCount();
        for (int i = 0; i < moves; i++) {
            action move;
            state.Actions.getAction(i, &move);
            double eval = Custy_Crew::Mini(state, move, bestMove, alpha, beta, depth - 1);
            
            if (eval > alpha) {
                alpha = eval;
                //maxMove = move;
                maxMove.fromCol = move.fromCol;
                maxMove.fromRow = move.fromRow;
                maxMove.toCol = move.toCol;
                maxMove.toRow = move.toRow;
                if (eval >= beta) {
                    break;
                }
            }
        }
        bestMove->fromCol = maxMove.fromCol;
        bestMove->fromRow = maxMove.fromRow;
        bestMove->toCol = maxMove.toCol;
        bestMove->toRow = maxMove.toRow;

        return alpha;
    }
}

void Custy_Crew::decideMove(gameState* state, action* Move, int maxDepth) {

    Custy_Crew::MiniMaxSearch(*state, Move, maxDepth);
    cout << "Count Evals: "<<countEvals << endl;
    return;
}

double Custy_Crew::evaluateState(gameState state) {
    double res = 0; //for now it is the sum of all peice+location values
    /*
    Peice values are assigned as:
        Pawn   =    1.00 point
        Knight =    3.20 points
        Bishop =    3.30 points
        Rook   =    5.00 points
        Queen  =    9.00 points

    Location values are given in .h file, as PSTs (peice-square tables) 
    with white at base (row index = 7 of table)
    */
    bool whiteTop = false; //to check weather the white side starts at top of board or is at base of board in layout
    if (state.Board.whiteHome == 0) {
        whiteTop = true; //white at top
    }
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int peice = state.Board.board[i][j];
            double peiceValue = 0;
            double locationValue = 0;
            //get values for peice on board[i][j]
            switch (peice) {
            case 1:
                peiceValue = 1;
                if(whiteTop)
                    locationValue = PawnPST[7 - i][j];
                else
                    locationValue = PawnPST[i][j];
                break;
            case -1:
                peiceValue = -1;
                if(whiteTop)
                    locationValue = -PawnPST[i][j];
                else
                    locationValue = -PawnPST[7 - i][j];
                break;
            case 2:
                peiceValue = 3.2;
                if (whiteTop)
                    locationValue = KnightPST[7 - i][j];
                else
                    locationValue = KnightPST[i][j];
                break;
            case -2:
                peiceValue = -3.2;
                if (whiteTop)
                    locationValue = -KnightPST[i][j];
                else
                    locationValue = -KnightPST[7 - i][j];
                break;
            case 3:
                peiceValue = 3.3;
                if (whiteTop)
                    locationValue = BishopPST[7 - i][j];
                else
                    locationValue = BishopPST[i][j];
                break;
            case -3:
                peiceValue = -3.3;
                if (whiteTop)
                    locationValue = -BishopPST[i][j];
                else
                    locationValue = -BishopPST[7 - i][j];
                break;
            case 4:
                peiceValue = 5;
                if (whiteTop)
                    locationValue = RookPST[7 - i][j];
                else
                    locationValue = RookPST[i][j];
                break;
            case -4:
                peiceValue = -5;
                if (whiteTop)
                    locationValue = -RookPST[i][j];
                else
                    locationValue = -RookPST[7 - i][j];
                break;
            case 5:
                peiceValue = 9;
                if (whiteTop)
                    locationValue = QueenPST[7 - i][j];
                else
                    locationValue = QueenPST[i][j];
                break;
            case -5:
                peiceValue = -9;
                if (whiteTop)
                    locationValue = -QueenPST[i][j];
                else
                    locationValue = -QueenPST[7 - i][j];
                break;
            case 6:
                peiceValue = 0;
                if (whiteTop)
                    locationValue = KingPST[7 - i][j];
                else
                    locationValue = KingPST[i][j];
                break;
            case -6:
                peiceValue = 0;
                if (whiteTop)
                    locationValue = -KingPST[i][j];
                else
                    locationValue = -KingPST[7 - i][j];
                break;
            default:
                peiceValue = 0;
                locationValue = 0;
            }
      
            //add total peice value to evaluation
            res += (peiceValue + locationValue);

        }
    }
    countEvals++;
    return res;
    //decrease eval for doubled pawns, open files, pawn islands  by 0.25
    //yea....no..too lazy and overworked i aint getting paid.

    //theres also mobility, center control, connectivity, king safety, trapped peices, tempo, patterns,
    //and all that stuff,
    //i tried to incorporate it into location tables and thas all i can do 
}