#include <iostream>
#include "MiniBoss.hpp"
#include "CombatAction.hpp"
using namespace std;

MiniBoss::MiniBoss() : Enemy("Guardian", Stats(5, 4, 3, 6), sf::Color(90, 140, 60), 60, 120), specialName("Smash"), specialDamage(15), specialInterval(4){
}

MiniBoss::MiniBoss(const string& name, const Stats& baseStats, const sf::Color& color, int rewardGold, int rewardExperience,
                   const string& specialName, int specialDamage, int specialInterval)
    : Enemy(name, baseStats, color, rewardGold, rewardExperience), specialName(specialName), specialDamage(specialDamage), specialInterval(specialInterval){
}

const string& MiniBoss::getSpecialName() const{
    return specialName;
}
int MiniBoss::getSpecialDamage() const{
    return specialDamage;
}
int MiniBoss::chooseAction(float distance, int turnNumber) const{
    if (turnNumber % specialInterval == 0){
        // every fourth attack use special attack
        return ACTION_SPECIAL; // warna calls the parent fucntion form enemy 
    }
    return Enemy::chooseAction(distance, turnNumber);
}

void MiniBoss::draw(sf::RenderWindow& window, const sf::Font& font) const{
    getSprite().draw(window, getCenterX(), GROUND_Y - getHeightAboveGround());
    drawLabel(window, font, sf::Color(255, 120, 100));
}// same as chatacter draw but red label color

string MiniBoss::getTitle() const{
    return "Mini Boss";
}
// creating a mini boss for the chapter
MiniBoss* MiniBoss::createForChapter(int chapter){
    MiniBoss* boss = nullptr;
    if (chapter == 2){
        boss = new MiniBoss("Frost Giant", Stats(9, 6, 2, 13), sf::Color(140, 200, 230), 120, 220, "Ice Boulder", 22, 4);
        boss->getSprite().setScale(4.5);
        boss->getSprite().loadTexture("assets/sprites/frost_giant.png");
    }
    // warna for chapter 1
    else{
        boss = new MiniBoss("Forest Guardian", Stats(6, 4, 3, 9), sf::Color(70, 130, 50), 60, 120, "Vine Lash", 14, 4);
        boss->getSprite().setScale(4.0);
        boss->getSprite().loadTexture("assets/sprites/forest_guardian.png");
    }
    return boss;
}

// need to add more chapters and there mini bosses for chap 3 and 4 for knightrunner and alchimest ones 
