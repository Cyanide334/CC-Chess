#include "..\include\Custy_Crew.h"
#include <iostream>
#include <iomanip>
#include "..\include\ENUM.h"
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

#define SKIP_MOVES 0

Custy_Crew::Custy_Crew(Color playerColor) :chessPlayer("Auto Player Name", playerColor) {

}


void Custy_Crew::MiniMaxSearch(gameState state, action* bestMove, int depth) {
    action Move;
    Move.fromCol = Move.fromRow = Move.toCol = Move.toRow = 0;

    auto start = high_resolution_clock::now();
    double eval = Custy_Crew::Max(state, Move, bestMove, INT_MIN, INT_MAX, depth);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    totalTime += duration.count();
    cout << "Move: " << endl;
    cout << "( " << bestMove->fromRow << "," << bestMove->fromCol << " )--"
        << "( " << bestMove->toRow << "," << bestMove->toCol << " ) " << endl;
    cout << "Evaluation: ";
    cout << fixed << setprecision(2) << eval;
    cout << " Posititions Evaluated: " << countEvals << " Duration: " << duration.count() << "ms" << endl;
    cout << "Total Time: " << totalTime << endl;
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
        //Just returning the evaluation at the last depth is pretty bad as in tactical positions, maybe the evry next move after the depth is disastrous.
        //To overcome this, it is better to perform a quiescience search at terminals which looks at all possible dequieting moves (e.g checks and captures)
        //This way, we can examine those moves and then return an appropriate evaluation. 
        //Again, im too lazy  and i aint doin this so yea.. positional eval is all u get.
        return Custy_Crew::evaluateState(state);
    }

    else {
        action minMove;
        minMove.fromRow = minMove.fromCol = minMove.toRow = minMove.toCol = 0;
        int moves = state.Actions.getActionCount();
        
        //lets order the moves first
        action stateActions[300];
        for (int i = 0; i < moves; i++) {
            state.Actions.getAction(i, &(stateActions[i]));
        }
        if(movesMade > SKIP_MOVES)
            orderMoves(state, stateActions, moves);

        double minEval = INT_MAX;
        for (int i = 0; i < moves; i++) {
            action move = stateActions[i];
            
            double eval = Custy_Crew::Max(state,move, bestMove,alpha, beta, depth - 1);
            if (minEval > eval) {
                minEval = eval;
                //minMove = move;
                minMove.fromCol = move.fromCol;
                minMove.fromRow = move.fromRow;
                minMove.toCol = move.toCol;
                minMove.toRow = move.toRow;
            }
            if (minEval == eval) {
                //check which move is better in case of tie by seeing peice captures for now i.e. MVV_LVA
                double a = evaluateMove(state, move);
                double b = evaluateMove(state, minMove);
                if (a < b) {
                    minMove.fromCol = move.fromCol;
                    minMove.fromRow = move.fromRow;
                    minMove.toCol = move.toCol;
                    minMove.toRow = move.toRow;
                }
            }
            if (minEval < beta) {
                beta = minEval;  
            }
            if (alpha >= beta) {
                break;
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
    if (!(Move.fromCol == Move.fromRow == Move.toCol == Move.toRow)) { //this is a gaurd condition so it doesnt apply move the first time its called.
        state.applyMove(Move);
    }
    if (state.Actions.getActionCount() == 0) {
        //do stalemates, white loss and black loss constitute no evaluation seedha inf or -inf eval? Probably so hell yea.
        if (state.kingUnderThreat(White))
            return -100;
        else if (state.kingUnderThreat(Black))
            return 100;
        else
            return -50;
    }
    
    else if (depth == 0) { //should do a queiescience search here for captures atleast..hmm..yea no
        return Custy_Crew::evaluateState(state);
    }
    else {
        action maxMove;
        maxMove.fromRow = maxMove.fromCol = maxMove.toRow = maxMove.toCol = 0;
        int moves = state.Actions.getActionCount();

        //lets order the moves first
        action stateActions[300];
        for (int i = 0; i < moves; i++) {
            state.Actions.getAction(i, &(stateActions[i]));
        }

        if (movesMade > SKIP_MOVES)
            orderMoves(state, stateActions, moves);

        double maxEval = INT_MIN;
        for (int i = 0; i < moves; i++) {
            action move = stateActions[i];
            
            double eval = Custy_Crew::Mini(state, move, bestMove, alpha, beta, depth - 1);
            if (maxEval < eval) {
                maxEval = eval;
                //maxMove = move;
                maxMove.fromCol = move.fromCol;
                maxMove.fromRow = move.fromRow;
                maxMove.toCol = move.toCol;
                maxMove.toRow = move.toRow;
            }
            if (maxEval == eval) {
                //check which move is better in case of tie by seeing peice captures for now
                double a = evaluateMove(state, move);
                double b = evaluateMove(state, maxMove);
                if (a > b) {
                    maxMove.fromCol = move.fromCol;
                    maxMove.fromRow = move.fromRow;
                    maxMove.toCol = move.toCol;
                    maxMove.toRow = move.toRow;
                }
            }
            if (maxEval > alpha) {
                alpha = maxEval;     
            }
            if (alpha >= beta) {
                break;
            }
        }
        bestMove->fromCol = maxMove.fromCol;
        bestMove->fromRow = maxMove.fromRow;
        bestMove->toCol = maxMove.toCol;
        bestMove->toRow = maxMove.toRow;
        return maxEval;
    }
}

void Custy_Crew::decideMove(gameState* state, action* Move, int maxDepth) {
    Custy_Crew::MiniMaxSearch(*state, Move, maxDepth); 
    movesMade++;
    return;
}

void Custy_Crew::orderMoves(gameState state, action stateActions[], int moves) {

    //put capture moves first
    int first = 0;
    while (evaluateMove(state, stateActions[first]) > 0) {
        first++;
        if (first == moves)
            return;
    }
    for (int i = first+1; i < moves; i++) {
        if (evaluateMove(state, stateActions[i]) > 0) {
            action tempMove = stateActions[i];
            stateActions[i] = stateActions[first];
            stateActions[first] = tempMove;
            first++;
        }
    }
  
    //bubble sort those capture moves
    bool swapped;
    for (int i = 0; i < first - 1; i++) {
        swapped = false;
        for (int j = 0; j < first - i - 1; j++) {
            if (evaluateMove(state, stateActions[j]) < evaluateMove(state, stateActions[j+1])) {
                action tempMove = stateActions[j];
                stateActions[j] = stateActions[j + 1];
                stateActions[j + 1] = tempMove;
                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }

}

double Custy_Crew::evaluateMove(gameState state, action Move) {

    //eval = captureValue[Victim][Attacker]
    double captureValue[7][7] = {
        {   0,   0,   0,   0,   0,   0,   0},
        {   0, 1.5, 1.4, 1.3, 1.2, 1.1,   1},
        {   0, 2.5, 2.4, 2.3, 2.2, 2.1,   2},
        {   0, 3.5, 3.4, 3.3, 3.2, 3.1,   3},
        {   0, 4.5, 4.4, 4.3, 4.2, 4.1,   4},
        {   0, 5.5, 5.4, 5.3, 5.2, 5.1,   5},
        {   0,   0,   0,   0,   0,   0,   0},
    };

    int Attacker = abs(state.Board.board[Move.fromRow][Move.fromCol]);
    int Victim = abs(state.Board.board[Move.toRow][Move.toCol]);

    return captureValue[Victim][Attacker];
}

double Custy_Crew::evaluateState(gameState state) {
    double res = 0; //for now it is the sum of all peice+location values
    
    
    //Peice values are assigned as:
    double
        PawnValue = 1.00,
        KnightValue = 3.20,
        BishopValue = 3.30,
        RookValue = 5.00,
        QueenValue = 9.00,
        KingValue = 0.00;


    //Peice-Square Tables with white at base (row index = 7 of table)
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
        {  -2, -1,  -1, -0.5, -0.5,  -1, -1,   -2},
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
        {-5,  -3,  -2,  -2,  -2,  -2,  -3, -5},
        {-4,  -2,   0,   0,   0,   0,  -2, -4},
        {-3,   0,   1, 1.5, 1.5,   1,   0, -3},
        {-3, 0.5, 1.5,   2,   2, 1.5, 0.5, -3},
        {-3,   0, 1.5,   2,   2, 1.5,   0, -3},
        {-3, 0.5,   1, 1.5, 1.5,   1, 0.5, -3},
        {-4,  -2,   0, 0.5, 0.5,   0,  -2, -4},
        {-5,  -3,  -2,  -2,  -2,  -2,  -3, -5}
    };
    double PawnPST[8][8] = {
        {  0,    0,  0,   0,   0,  0,    0,   0},
        {  5,    5,  5,   5,   5,  5,    5,   5},
        {  1,    1,  2,   3,   3,  2,    1,   1},
        {0.5,  0.5,  1, 2.5, 2.5,  1,  0.5, 0.5},
        {  0,    0,  0,2.25,2.25,  0,    0,   0},
        {0.5, -0.5, -1,   0,   0, -1, -0.5, 0.5},
        {0.5,    1,  1,  -2,  -2,  1,    1, 0.5},
        {  0,    0,  0,   0,   0,  0,    0,   0}
    };

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
                peiceValue = PawnValue;
                if(whiteTop)
                    locationValue = PawnPST[7 - i][j];
                else
                    locationValue = PawnPST[i][j];
                break;
            case -1:
                peiceValue = -PawnValue;
                if(whiteTop)
                    locationValue = -PawnPST[i][j];
                else
                    locationValue = -PawnPST[7 - i][j];
                break;
            case 2:
                peiceValue = KnightValue;
                if (whiteTop)
                    locationValue = KnightPST[7 - i][j];
                else
                    locationValue = KnightPST[i][j];
                break;
            case -2:
                peiceValue = -KnightValue;
                if (whiteTop)
                    locationValue = -KnightPST[i][j];
                else
                    locationValue = -KnightPST[7 - i][j];
                break;
            case 3:
                peiceValue = BishopValue;
                if (whiteTop)
                    locationValue = BishopPST[7 - i][j];
                else
                    locationValue = BishopPST[i][j];
                break;
            case -3:
                peiceValue = -BishopValue;
                if (whiteTop)
                    locationValue = -BishopPST[i][j];
                else
                    locationValue = -BishopPST[7 - i][j];
                break;
            case 4:
                peiceValue = RookValue;
                if (whiteTop)
                    locationValue = RookPST[7 - i][j];
                else
                    locationValue = RookPST[i][j];
                break;
            case -4:
                peiceValue = -RookValue;
                if (whiteTop)
                    locationValue = -RookPST[i][j];
                else
                    locationValue = -RookPST[7 - i][j];
                break;
            case 5:
                peiceValue = QueenValue;
                if (whiteTop)
                    locationValue = QueenPST[7 - i][j];
                else
                    locationValue = QueenPST[i][j];
                break;
            case -5:
                peiceValue = -QueenValue;
                if (whiteTop)
                    locationValue = -QueenPST[i][j];
                else
                    locationValue = -QueenPST[7 - i][j];
                break;
            case 6:
                peiceValue = KingValue;
                if (whiteTop)
                    locationValue = KingPST[7 - i][j];
                else
                    locationValue = KingPST[i][j];
                break;
            case -6:
                peiceValue = -KingValue;
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
    //yea....no..too lazy and overworked.. i aint getting paid.

    //theres also mobility, center control, connectivity, king safety, trapped peices, tempo, patterns,
    //and all that stuff,
    //i tried to incorporate it into location tables and thas all i can do so yea..
}

