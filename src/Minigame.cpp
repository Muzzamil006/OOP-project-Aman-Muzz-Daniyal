#include <iostream>
#include "Minigame.hpp"

using namespace std;

Minigame::Minigame() : name("Minigame"), instructions(""), timeLimit(30.0f), timeLeft(30.0f), score(0), finished(false), won(false), playerSprite(nullptr){
}

/*
DevilsDeep constructor calls this base-class constructor
Store the minigame name, instructions and time limit
Initialize timeLeft, score, finished and won
Start with no player sprite assigned
*/
Minigame::Minigame(const string& name, const string& instructions, float timeLimit)
    : name(name), instructions(instructions), timeLimit(timeLimit), timeLeft(timeLimit), score(0), finished(false), won(false), playerSprite(nullptr){
}

/*
MinigameState deletes its Minigame pointer
The virtual destructor allows the actual derived object to be destroyed correctly
The base destructor then runs; it does not delete the borrowed hero sprite
*/
Minigame::~Minigame(){
}

/*
DevilsDeep::update(dt) calls tickTimer(dt)
Subtract elapsed seconds from timeLeft
Clamp timeLeft to zero if it becomes negative
*/
void Minigame::tickTimer(float dt){
    timeLeft -= dt;
    if (timeLeft < 0.0f){
        timeLeft = 0.0f;
    }
}

/*
DevilsDeep detects an enemy collision or timeout
Pass whether score meets the required coin target
Set finished to true and won to the supplied result
MinigameState detects completion and shows the result screen
*/
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

/*
MinigameState gets the selected hero sprite from Game
Store a borrowed pointer to that sprite
DevilsDeep::draw() uses it to display the selected hero
*/
void Minigame::setPlayerSprite(const CharacterSprite* sprite){
    playerSprite = sprite;
}

/*
MinigameState requests the reward for the current result
If won, calculate score * 3 + 20; otherwise calculate score * 2
Return the amount without changing the hero gold
The result screen displays it; clicking Continue adds it to the hero
*/
int Minigame::getGoldReward() const{
    if (won){
        return score * 3 + 20;
    }
    return score * 2;
}
