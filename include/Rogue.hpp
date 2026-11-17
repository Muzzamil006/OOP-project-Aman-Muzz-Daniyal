#ifndef ROGUE_HPP
#define ROGUE_HPP

#include <iostream>
#include "Hero.hpp"

using namespace std;

class Rogue : public Hero{
public:
    Rogue();
    Rogue(const string& name);

    int useSpecial(Character& target) override;
    string getSpecialName() const override;
    float getSpecialRange() const override;
    int getHeroType() const override;
    string getTitle() const override;
};

#endif
