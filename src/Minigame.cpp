#include <iostream>
#include "Minigame.hpp"

using namespace std;

Minigame::Minigame() : name("Minigame"), instructions(""), timeLimit(30.0f), timeLeft(30.0f), score(0), finished(false), won(false), playerSprite(nullptr){
}

Minigame::Minigame(const string& name, const string& instructions, float timeLimit)
    : name(name), instructions(instructions), timeLimit(timeLimit), timeLeft(timeLimit), score(0), finished(false), won(false), playerSprite(nullptr){
}

Minigame::~Minigame(){
}

void Minigame::tickTimer(float dt){
    timeLeft -= dt;
    if (timeLeft < 0.0f){
        timeLeft = 0.0f;
    }
}

void Minigame::finish(bool playerWon){
    finished = true;
    won = playerWon;
}

const string& Minigame::getName() const{
    return name;
}

const string& Minigame::getInstructions() const{
    return instructions;
}

float Minigame::getTimeLeft() const{
    return timeLeft;
}

int Minigame::getScore() const{
    return score;
}

bool Minigame::isFinished() const{
    return finished;
}

bool Minigame::isWon() const{
    return won;
}

void Minigame::setPlayerSprite(const CharacterSprite* sprite){
    playerSprite = sprite;
}

int Minigame::getGoldReward() const{
    if (won){
        return score * 3 + 20;
    }
    return score * 2;
}
