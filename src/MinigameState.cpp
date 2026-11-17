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

MinigameState::MinigameState(Game* game, Minigame* minigame)
    : GameState(game), minigame(minigame), phase(PHASE_INTRO),
      startButton("Start", game->getFont(), 380.0f, 380.0f, 200.0f, 45.0f),
      retryButton("Retry (-" + to_string(RETRY_COST) + " gold)", game->getFont(), 270.0f, 400.0f, 200.0f, 45.0f),
      continueButton("Continue", game->getFont(), 490.0f, 400.0f, 200.0f, 45.0f){
    startButton.setColor(sf::Color(190, 140, 40));
    continueButton.setColor(sf::Color(190, 140, 40));
    minigame->setPlayerSprite(&game->getHero()->getSprite());
}

MinigameState::~MinigameState(){
    delete minigame;
}

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
