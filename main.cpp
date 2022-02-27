#include "include\actionList.h"
#include <iostream>
#include <iomanip>
#include "include\chess.h"
#include "include\autoPlayer.h"
#include "include\humanPlayer.h"
#include "include\Custy_Crew.h"

using namespace std;

int main(){
    chess Game;
    Game.Players[0] = new humanPlayer("Human Player Name", Black);
    //Game.Players[0] = new autoPlayer(Black);
    Game.Players[1] = new Custy_Crew();
    Game.playGame();
    return 0;
}
