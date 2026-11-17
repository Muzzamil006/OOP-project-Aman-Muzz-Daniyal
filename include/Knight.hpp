#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include <iostream>
#include "Hero.hpp"

using namespace std;

class Knight : public Hero{
public:
    Knight();
    Knight(const string& name);

    int useSpecial(Character& target) override;
    string getSpecialName() const override;
    float getSpecialRange() const override;
    int getHeroType() const override;
    string getTitle() const override;
};

#endif
