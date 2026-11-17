#ifndef MINIGAMEFACTORY_HPP
#define MINIGAMEFACTORY_HPP

#include <iostream>

using namespace std;

class Minigame;

class MinigameFactory{
private:
    int extraCoinsPerChapter;

public:
    MinigameFactory();
    MinigameFactory(int extraCoinsPerChapter);

    int getExtraCoinsPerChapter() const;
    Minigame* create(int chapter) const;
};

#endif
