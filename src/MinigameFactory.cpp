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

Minigame* MinigameFactory::create(int chapter) const{
    int coinsNeeded = 8 + (chapter - 1) * extraCoinsPerChapter;
    return new DevilsDeep(30.0f, coinsNeeded, 4 + chapter);
}
