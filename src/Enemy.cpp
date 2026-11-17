#include <iostream>
#include "Enemy.hpp"
#include "CombatAction.hpp"
using namespace std;

const float Enemy::MELEE_RANGE = 150.0;

Enemy::Enemy() : Character("Bandit", Stats(3, 2, 3, 3), 750.0, sf::Color(150, 60, 60)), rewardGold(10), rewardExperience(20){
    getSprite().setFacingRight(false);
}
Enemy::Enemy(const string& name, const Stats& baseStats, const sf::Color& color, int rewardGold, int rewardExperience)
    : Character(name, baseStats, 750.0, color), rewardGold(rewardGold), rewardExperience(rewardExperience){
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
// the ai for the enemy to choose an action based on distance and turn number
int Enemy::chooseAction(float distance, int turnNumber) const{
    if (distance > MELEE_RANGE){ // more than 150 far way so walk over to player
        return ACTION_ADVANCE;
    }
    if (getHealth() < getMaxHealth() / 3 && turnNumber % 5 == 0){
        return ACTION_REST; // hp less than 1/3 so rest every 5 turns so can heal
    }
    if (turnNumber % 3 == 0){
        return ACTION_POWER_ATTACK; // every 3turns a power attack
    }
    return ACTION_QUICK_ATTACK; // or else normal attack
}
string Enemy::getTitle() const{
    return "Bandit";
}
