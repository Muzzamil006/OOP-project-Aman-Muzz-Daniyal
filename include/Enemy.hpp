#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <iostream>
#include "Character.hpp"

using namespace std;

class Enemy : public Character{
private:
    int rewardGold;
    int rewardExperience;

public:
    static const float MELEE_RANGE;

    Enemy();
    Enemy(const string& name, const Stats& baseStats, const sf::Color& color, int rewardGold, int rewardExperience);
    virtual ~Enemy();

    int getRewardGold() const;
    int getRewardExperience() const;

    virtual int chooseAction(float distance, int turnNumber) const;
    string getTitle() const override;
};

#endif
