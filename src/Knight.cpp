#include <iostream>
#include "Knight.hpp"
using namespace std;

Knight::Knight() : Hero("Amaan", Stats(6, 5, 3, 5), sf::Color(70, 110, 220)){
    getSprite().loadTexture("assets/sprites/amaan.png", 17.5, 65.0);
    getSprite().loadArmour("assets/sprites/armour.png");
}
Knight::Knight(const string& name) : Hero(name, Stats(6, 5, 3, 5), sf::Color(70, 110, 220)){
    getSprite().loadTexture("assets/sprites/amaan.png", 17.5, 65.0);
    getSprite().loadArmour("assets/sprites/armour.png");
}
//sheildbash is my speacial ability does damage and pushes the enemy back by 60px
int Knight::useSpecial(Character& target){
    int damage = getTotalStats().getStrength() * 4 + 5 - target.getTotalStats().getDefence();
    if (damage < 1){
        damage = 1;
    }
    target.takeDamage(damage);
    target.moveBy(60.0);
    return damage;
}
string Knight::getSpecialName() const{
    return "Shield Bash";
}

float Knight::getSpecialRange() const{
    return 150.0;
}
int Knight::getHeroType() const{
    return KNIGHT;
}
string Knight::getTitle() const{
    return "Knight";
}
