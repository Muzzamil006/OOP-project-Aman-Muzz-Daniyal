#include <iostream>
#include "Hero.hpp"
#include "Knight.hpp"
#include "Mage.hpp"
#include "Rogue.hpp"

using namespace std;

Hero::Hero() : Character(), level(1), experience(0), gold(50), statPoints(0), shards(0), inventory(){
}

Hero::Hero(const string& name, const Stats& baseStats, const sf::Color& color)
    : Character(name, baseStats, 150.0f, color), level(1), experience(0), gold(50), statPoints(0), shards(0), inventory(){
}

Hero::~Hero(){
}

Stats Hero::getTotalStats() const{
    return getBaseStats() + inventory.getTotalBonus();
}

int Hero::getLevel() const{
    return level;
}

int Hero::getExperience() const{
    return experience;
}

int Hero::getExperienceToNextLevel() const{
    return level * 100;
}

int Hero::getGold() const{
    return gold;
}

int Hero::getStatPoints() const{
    return statPoints;
}

int Hero::getShards() const{
    return shards;
}

Inventory& Hero::getInventory(){
    return inventory;
}

const Inventory& Hero::getInventory() const{
    return inventory;
}

void Hero::addGold(int amount){
    gold += amount;
}

bool Hero::spendGold(int amount){
    if (amount > gold){
        return false;
    }
    gold -= amount;
    return true;
}

bool Hero::addExperience(int amount){
    bool levelledUp = false;
    experience += amount;
    while (experience >= getExperienceToNextLevel()){
        experience -= getExperienceToNextLevel();
        level += 1;
        statPoints += 3;
        levelledUp = true;
    }
    return levelledUp;
}

bool Hero::spendStatPoint(int statIndex){
    if (statPoints <= 0){
        return false;
    }
    upgradeStat(statIndex);
    statPoints -= 1;
    return true;
}

void Hero::addShard(){
    shards += 1;
}

void Hero::setProgress(int level, int experience, int gold, int statPoints, int shards){
    this->level = level;
    this->experience = experience;
    this->gold = gold;
    this->statPoints = statPoints;
    this->shards = shards;
}

void Hero::refreshAppearance(){
    getSprite().setShowArmour(inventory.getTotalBonus().getDefence() >= 2);
}

Hero* Hero::create(int heroType){
    if (heroType == MAGE){
        return new Mage();
    }
    if (heroType == ROGUE){
        return new Rogue();
    }
    return new Knight();
}
