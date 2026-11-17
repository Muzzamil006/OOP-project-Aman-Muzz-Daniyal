#include <iostream>
#include "GameOverState.hpp"
#include "Game.hpp"
#include "Hero.hpp"
using namespace std;
GameOverState::GameOverState() : GameState(), message(""), canRetry(false), retryButton(), menuButton(){
}

GameOverState::GameOverState(Game* game, const string& message, bool canRetry)
    : GameState(game), message(message), canRetry(canRetry),
      retryButton("Retry Fight", game->getFont(), 380.0f, 330.0f, 200.0f, 45.0f),
      menuButton("Main Menu", game->getFont(), 380.0f, 390.0f, 200.0f, 45.0f){
    retryButton.setColor(sf::Color(190, 140, 40));
    retryButton.setEnabled(canRetry);
}

void GameOverState::handleInput(const sf::Event& event){
    if (canRetry && retryButton.isClicked(event)){
        game->startArena();
    }
    else if (menuButton.isClicked(event)){
        game->showMenu();
    }
}
void GameOverState::update(float dt){
}

void GameOverState::draw(sf::RenderWindow& window) const{
    sf::RectangleShape background(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    background.setFillColor(canRetry ? sf::Color(10, 5, 5) : sf::Color(30, 30, 70));
    window.draw(background);
    if (canRetry){
        drawCenteredText(window, "GAME OVER", 56, 150.0f, sf::Color(220, 60, 60));
    }
    else{
        drawCenteredText(window, "TO BE CONTINUED", 48, 150.0f, sf::Color(240, 200, 80));
    }
    drawCenteredText(window, message, 20, 240.0f, sf::Color::White);
    const Hero* hero = game->getHero();
    if (hero != nullptr){
        drawCenteredText(window, "Level " + to_string(hero->getLevel()) + "   Gold " + to_string(hero->getGold()) + "   Shards " + to_string(hero->getShards()) + " / 4", 18, 280.0f, sf::Color(200, 200, 200));
    }
    if (canRetry){
        retryButton.draw(window);
    }
    menuButton.draw(window);
}
