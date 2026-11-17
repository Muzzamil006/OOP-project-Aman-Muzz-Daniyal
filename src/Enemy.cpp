#include <iostream>
#include "Enemy.hpp"
#include "CombatAction.hpp"

using namespace std;

const float Enemy::MELEE_RANGE = 150.0f;

Enemy::Enemy() : Character("Bandit", Stats(3, 2, 3, 3), 750.0f, sf::Color(150, 60, 60)), rewardGold(10), rewardExperience(20){
    getSprite().setFacingRight(false);
}

Enemy::Enemy(const string& name, const Stats& baseStats, const sf::Color& color, int rewardGold, int rewardExperience)
    : Character(name, baseStats, 750.0f, color), rewardGold(rewardGold), rewardExperience(rewardExperience){
    getSprite().setFacingRight(false);
}

Enemy::~Enemy(){
}

int Enemy::getRewardGold() const{
    return rewardGold;
}

int Enemy::getRewardExperience() const{
    return rewardExperience;
}

int Enemy::chooseAction(float distance, int turnNumber) const{
    if (distance > MELEE_RANGE){
        return ACTION_ADVANCE;
    }
    if (getHealth() < getMaxHealth() / 3 && turnNumber % 5 == 0){
        return ACTION_REST;
    }
    if (turnNumber % 3 == 0){
        return ACTION_POWER_ATTACK;
    }
    return ACTION_QUICK_ATTACK;
}

string Enemy::getTitle() const{
    return "Bandit";
}
