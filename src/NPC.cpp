#include <iostream>
#include "NPC.hpp"

using namespace std;

NPC::NPC() : Character("Villager", Stats(1, 1, 1, 1), 700.0f, sf::Color(200, 170, 60)), role("Villager"){
    getSprite().setFacingRight(false);
}

NPC::NPC(const string& name, const string& role, const sf::Color& color)
    : Character(name, Stats(1, 1, 1, 1), 700.0f, color), role(role){
    getSprite().setFacingRight(false);
}

const string& NPC::getRole() const{
    return role;
}

string NPC::getTitle() const{
    return role;
}
