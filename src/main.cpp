#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.hpp"

using namespace std;

int main(){
    srand((unsigned int)time(nullptr));
    Game game(WINDOW_WIDTH, WINDOW_HEIGHT, "Teen Talwar");
    game.run();
    return 0;
}
