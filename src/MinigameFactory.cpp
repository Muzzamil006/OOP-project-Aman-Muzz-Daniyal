#include <iostream>
#include "MinigameFactory.hpp"
#include "DevilsDeep.hpp"

using namespace std;

MinigameFactory::MinigameFactory() : extraCoinsPerChapter(4){
}

MinigameFactory::MinigameFactory(int extraCoinsPerChapter) : extraCoinsPerChapter(extraCoinsPerChapter){
}

int MinigameFactory::getExtraCoinsPerChapter() const{
    return extraCoinsPerChapter;
}

/*
StoryState finishes the chapter story and calls Game::startMinigame()
Game asks this factory to create a minigame for the current chapter
Calculate coinsNeeded as 8 + (chapter - 1) * extraCoinsPerChapter
Create DevilsDeep with 30 seconds and 4 + chapter enemies
Return the DevilsDeep object through a Minigame pointer
Game passes that pointer to the new MinigameState
*/
Minigame* MinigameFactory::create(int chapter) const{
    int coinsNeeded = 8 + (chapter - 1) * extraCoinsPerChapter;
    return new DevilsDeep(30.0f, coinsNeeded, 4 + chapter);
}
