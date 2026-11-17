#include <iostream>
#include "Rogue.hpp"

using namespace std;

Rogue::Rogue() : Hero("Muzzamil", Stats(5, 3, 7, 3), sf::Color(60, 170, 90)){
    getSprite().loadTexture("assets/sprites/muzz.png", 17.5f, 65.0f);
    getSprite().loadArmour("assets/sprites/armour.png");
}

Rogue::Rogue(const string& name) : Hero(name, Stats(5, 3, 7, 3), sf::Color(60, 170, 90)){
    getSprite().loadTexture("assets/sprites/muzz.png", 17.5f, 65.0f);
    getSprite().loadArmour("assets/sprites/armour.png");
}

int Rogue::useSpecial(Character& target){
    setPositionX(target.getPositionX() - 125.0f);
    int damage = getTotalStats().getStrength() * 2 + getTotalStats().getAgility() * 3 - target.getTotalStats().getDefence();
    if (damage < 1){
        damage = 1;
    }
    target.takeDamage(damage);
    return damage;
}

string Rogue::getSpecialName() const{
    return "Dash Strike";
}

float Rogue::getSpecialRange() const{
    return 450.0f;
}

int Rogue::getHeroType() const{
    return ROGUE;
}

string Rogue::getTitle() const{
    return "Rogue";
}
