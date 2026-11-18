#include <iostream>
#include "MinigameState.hpp"
#include "Game.hpp"
#include "Hero.hpp"
#include "Minigame.hpp"
#include "DevilsDeep.hpp"
#include "SoundManager.hpp"

using namespace std;

MinigameState::MinigameState() : GameState(), minigame(new DevilsDeep()), phase(PHASE_INTRO), startButton(), retryButton(), continueButton(){
}

/*
Game receives a gameplay object from MinigameFactory
Pass Game and the Minigame pointer into this constructor
Store the minigame pointer and begin in PHASE_INTRO
Create Start, Retry and Continue buttons
Give the minigame a borrowed pointer to the selected hero sprite
Once this state becomes active, draw() shows the introduction
*/
MinigameState::MinigameState(Game* game, Minigame* minigame)
    : GameState(game), minigame(minigame), phase(PHASE_INTRO),
      startButton("Start", game->getFont(), 380.0f, 380.0f, 200.0f, 45.0f),
      retryButton("Retry (-" + to_string(RETRY_COST) + " gold)", game->getFont(), 270.0f, 400.0f, 200.0f, 45.0f),
      continueButton("Continue", game->getFont(), 490.0f, 400.0f, 200.0f, 45.0f){
    startButton.setColor(sf::Color(190, 140, 40));
    continueButton.setColor(sf::Color(190, 140, 40));
    minigame->setPlayerSprite(&game->getHero()->getSprite());
}

/*
Game replaces the minigame screen with another state
Destroy this MinigameState
Delete the owned gameplay object through its Minigame pointer
The virtual base destructor ensures correct derived-object cleanup
*/
MinigameState::~MinigameState(){
    delete minigame;
}

/*
Game loop passes an input event to this screen
During INTRO, Start or Enter calls minigame->reset() and selects PLAYING
During RESULT, Retry spends the retry cost and resets the same gameplay object
Alternatively, Continue adds getGoldReward() to the shared hero
Continue asks Game to open the shop
Held movement keys are handled separately by DevilsDeep::update()
*/
void MinigameState::handleInput(const sf::Event& event){
    if (phase == PHASE_INTRO){
        if (startButton.isClicked(event) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)){
            SoundManager::getInstance().playClick();
            minigame->reset();
            phase = PHASE_PLAYING;
        }
    }
    else if (phase == PHASE_RESULT){
        if (retryButton.isClicked(event) && game->getHero()->spendGold(RETRY_COST)){
            minigame->reset();
            phase = PHASE_PLAYING;
        }
        else if (continueButton.isClicked(event)){
            game->getHero()->addGold(minigame->getGoldReward());
            game->startShop();
        }
    }
}

/*
Game loop calls this update(dt) each frame
Update the shared hero
Only during PHASE_PLAYING, call minigame->update(dt)
Virtual dispatch runs DevilsDeep::update(dt)
Check minigame->isFinished() after that update
If finished, switch to PHASE_RESULT and enable Retry only when affordable
*/
void MinigameState::update(float dt){
    game->getHero()->update(dt);
    if (phase == PHASE_PLAYING){
        minigame->update(dt);
        if (minigame->isFinished()){
            phase = PHASE_RESULT;
            retryButton.setEnabled(game->getHero()->getGold() >= RETRY_COST);
        }
    }
}

/*
Game loop calls this draw() for the active minigame screen
Draw the minigame name
During INTRO, draw instructions, time limit and Start, then return
Otherwise call minigame->draw(), which runs DevilsDeep::draw()
During RESULT, draw the outcome panel over the gameplay area
Show score, calculated reward, Retry and Continue
*/
void MinigameState::draw(sf::RenderWindow& window) const{
    drawText(window, minigame->getName(), 26, 40.0f, 15.0f, sf::Color(240, 200, 80));
    if (phase == PHASE_INTRO){
        drawPanel(window, 180.0f, 150.0f, 600.0f, 300.0f);
        drawCenteredText(window, minigame->getName(), 36, 180.0f, sf::Color(240, 200, 80));
        drawCenteredText(window, minigame->getInstructions(), 17, 260.0f, sf::Color::White);
        drawCenteredText(window, "Time limit: " + to_string((int)minigame->getTimeLeft()) + " seconds", 17, 300.0f, sf::Color(200, 200, 200));
        startButton.draw(window);
        return;
    }
    minigame->draw(window, game->getFont());
    if (phase == PHASE_RESULT){
        drawPanel(window, 230.0f, 200.0f, 500.0f, 270.0f);
        if (minigame->isWon()){
            drawCenteredText(window, "You made it out!", 32, 225.0f, sf::Color(130, 220, 130));
        }
        else{
            drawCenteredText(window, "The Evil Aarijs got you!", 32, 225.0f, sf::Color(230, 90, 90));
        }
        drawCenteredText(window, "Coins collected: " + to_string(minigame->getScore()), 20, 290.0f, sf::Color::White);
        drawCenteredText(window, "Reward: " + to_string(minigame->getGoldReward()) + " gold", 20, 325.0f, sf::Color(240, 200, 80));
        retryButton.draw(window);
        continueButton.draw(window);
    }
}
