#ifndef MINIBOSS_HPP
#define MINIBOSS_HPP
#include <iostream>
#include "Enemy.hpp"
using namespace std;

class MiniBoss : public Enemy{
private:
    string specialName;
    int specialDamage;
    int specialInterval;

public:
    MiniBoss();
    MiniBoss(const string& name, const Stats& baseStats, const sf::Color& color, int rewardGold, int rewardExperience,
             const string& specialName, int specialDamage, int specialInterval);
    const string& getSpecialName() const;
    int getSpecialDamage() const;
    int chooseAction(float distance, int turnNumber) const override;
    void draw(sf::RenderWindow& window, const sf::Font& font) const override;
    string getTitle() const override;
    static MiniBoss* createForChapter(int chapter);
};

#endif
