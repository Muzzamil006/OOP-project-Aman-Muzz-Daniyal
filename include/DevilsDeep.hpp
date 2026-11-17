#ifndef DEVILSDEEP_HPP
#define DEVILSDEEP_HPP

#include <iostream>
#include <vector>
#include "Minigame.hpp"

using namespace std;

class DevilsDeep : public Minigame{
private:
    sf::Vector2f playerPosition;
    vector<sf::Vector2f> coins;
    vector<sf::Vector2f> aarijPositions;
    vector<sf::Vector2f> aarijVelocities;
    int coinsNeeded;
    int aarijCount;

    sf::Vector2f randomPoint(float minX) const;

public:
    static const float LEFT;
    static const float TOP;
    static const float RIGHT;
    static const float BOTTOM;

    DevilsDeep();
    DevilsDeep(float timeLimit, int coinsNeeded, int aarijCount);

    void reset() override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window, const sf::Font& font) const override;
};

#endif
