#ifndef MINIGAME_HPP
#define MINIGAME_HPP

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class CharacterSprite;

class Minigame{
protected:
    string name;
    string instructions;
    float timeLimit;
    float timeLeft;
    int score;
    bool finished;
    bool won;
    const CharacterSprite* playerSprite;

    void tickTimer(float dt);
    void finish(bool playerWon);

public:
    Minigame();
    Minigame(const string& name, const string& instructions, float timeLimit);
    virtual ~Minigame();

    const string& getName() const;
    const string& getInstructions() const;
    float getTimeLeft() const;
    int getScore() const;
    bool isFinished() const;
    bool isWon() const;
    void setPlayerSprite(const CharacterSprite* sprite);

    virtual void reset() = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window, const sf::Font& font) const = 0;
    virtual int getGoldReward() const;
};

#endif
