#include <iostream>
#include "Stats.hpp"

using namespace std;

Stats::Stats() : strength(0), defence(0), agility(0), vitality(0){
}

Stats::Stats(int strength, int defence, int agility, int vitality)
    : strength(strength), defence(defence), agility(agility), vitality(vitality){
}

int Stats::getStrength() const{
    return strength;
}

int Stats::getDefence() const{
    return defence;
}

int Stats::getAgility() const{
    return agility;
}

int Stats::getVitality() const{
    return vitality;
}

void Stats::increase(int statIndex){
    increase(statIndex, 1);
}

void Stats::increase(int statIndex, int amount){
    if (statIndex == STRENGTH){
        strength += amount;
    }
    else if (statIndex == DEFENCE){
        defence += amount;
    }
    else if (statIndex == AGILITY){
        agility += amount;
    }
    else if (statIndex == VITALITY){
        vitality += amount;
    }
}

Stats Stats::operator+(const Stats& other) const{
    return Stats(strength + other.strength, defence + other.defence, agility + other.agility, vitality + other.vitality);
}

Stats& Stats::operator+=(const Stats& other){
    strength += other.strength;
    defence += other.defence;
    agility += other.agility;
    vitality += other.vitality;
    return *this;
}

bool Stats::operator==(const Stats& other) const{
    return strength == other.strength && defence == other.defence && agility == other.agility && vitality == other.vitality;
}

ostream& operator<<(ostream& out, const Stats& stats){
    out << stats.strength << " " << stats.defence << " " << stats.agility << " " << stats.vitality;
    return out;
}
