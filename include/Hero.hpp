#ifndef HERO_HPP
#define HERO_HPP

#include <iostream>
#include "Character.hpp"
#include "Inventory.hpp"

using namespace std;

class Hero : public Character{
private:
    int level;
    int experience;
    int gold;
    int statPoints;
    int shards;
    Inventory inventory;

public:
    static const int KNIGHT = 0;
    static const int MAGE = 1;
    static const int ROGUE = 2;

    Hero();
    Hero(const string& name, const Stats& baseStats, const sf::Color& color);
    virtual ~Hero();

    Stats getTotalStats() const override;

    int getLevel() const;
    int getExperience() const;
    int getExperienceToNextLevel() const;
    int getGold() const;
    int getStatPoints() const;
    int getShards() const;
    Inventory& getInventory();
    const Inventory& getInventory() const;

    void addGold(int amount);
    bool spendGold(int amount);
    bool addExperience(int amount);
    bool spendStatPoint(int statIndex);
    void addShard();
    void setProgress(int level, int experience, int gold, int statPoints, int shards);
    void refreshAppearance();

    virtual int useSpecial(Character& target) = 0;
    virtual string getSpecialName() const = 0;
    virtual float getSpecialRange() const = 0;
    virtual int getHeroType() const = 0;

    static Hero* create(int heroType);
};

#endif
