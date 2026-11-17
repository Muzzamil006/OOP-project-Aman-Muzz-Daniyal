#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Stats.hpp"
#include "CharacterSprite.hpp"

using namespace std;

class Character{
private:
    string name;
    Stats baseStats;
    int health;
    int maxHealth;
    float positionX;
    float heightAboveGround;
    sf::Color color;
    CharacterSprite sprite;
    static int characterCount;

protected:
    void drawLabel(sf::RenderWindow& window, const sf::Font& font, const sf::Color& labelColor) const;

public:
    static const float GROUND_Y;

    Character();
    Character(const string& name, const Stats& baseStats, float positionX, const sf::Color& color);
    virtual ~Character();

    const string& getName() const;
    const Stats& getBaseStats() const;
    virtual Stats getTotalStats() const;
    int getHealth() const;
    int getMaxHealth() const;
    float getPositionX() const;
    const sf::Color& getColor() const;
    float getCenterX() const;
    float getHeightAboveGround() const;
    CharacterSprite& getSprite();
    const CharacterSprite& getSprite() const;
    void update(float dt);
    bool isAlive() const;

    void setBaseStats(const Stats& stats);
    void setPositionX(float x);
    void setHeightAboveGround(float height);
    void moveBy(float amount);
    void upgradeStat(int statIndex);

    void takeDamage(int amount);
    void takeDamage(int amount, float multiplier);
    void heal(int amount);
    void restoreFullHealth();
    void recalculateMaxHealth();

    virtual void draw(sf::RenderWindow& window, const sf::Font& font) const;
    virtual string getTitle() const = 0;

    static int getCharacterCount();
};

#endif
