#ifndef STATS_HPP
#define STATS_HPP

#include <iostream>

using namespace std;

class Stats{
private:
    int strength;
    int defence;
    int agility;
    int vitality;

public:
    static const int STRENGTH = 0;
    static const int DEFENCE = 1;
    static const int AGILITY = 2;
    static const int VITALITY = 3;

    Stats();
    Stats(int strength, int defence, int agility, int vitality);

    int getStrength() const;
    int getDefence() const;
    int getAgility() const;
    int getVitality() const;

    void increase(int statIndex);
    void increase(int statIndex, int amount);

    Stats operator+(const Stats& other) const;
    Stats& operator+=(const Stats& other);
    bool operator==(const Stats& other) const;

    friend ostream& operator<<(ostream& out, const Stats& stats);
};

#endif
