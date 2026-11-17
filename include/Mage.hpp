#ifndef MAGE_HPP
#define MAGE_HPP

#include <iostream>
#include "Hero.hpp"

using namespace std;

class Mage : public Hero{
public:
    Mage();
    Mage(const string& name);

    int useSpecial(Character& target) override;
    string getSpecialName() const override;
    float getSpecialRange() const override;
    int getHeroType() const override;
    string getTitle() const override;
};

#endif
