#include <iostream>
#include "Mage.hpp"

using namespace std;

Mage::Mage() : Hero("Daniyal", Stats(4, 3, 5, 4), sf::Color(150, 80, 200)){
    getSprite().loadTexture("assets/sprites/dani.png", 17.5f, 65.0f);
    getSprite().loadArmour("assets/sprites/armour.png");
}

Mage::Mage(const string& name) : Hero(name, Stats(4, 3, 5, 4), sf::Color(150, 80, 200)){
    getSprite().loadTexture("assets/sprites/dani.png", 17.5f, 65.0f);
    getSprite().loadArmour("assets/sprites/armour.png");
}

int Mage::useSpecial(Character& target){
    int damage = 25 + getTotalStats().getAgility() * 2;
    target.takeDamage(damage);
    return damage;
}

string Mage::getSpecialName() const{
    return "Fireball";
}

float Mage::getSpecialRange() const{
    return 1000.0f;
}

int Mage::getHeroType() const{
    return MAGE;
}

string Mage::getTitle() const{
    return "Mage";
}
