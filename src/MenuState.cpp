#include <iostream>
#include "MenuState.hpp"
#include "Game.hpp"
#include "SoundManager.hpp"

using namespace std;

MenuState::MenuState() : GameState(), buttons(), showingInstructions(false), message(""){
}

MenuState::MenuState(Game* game) : GameState(game), buttons(), showingInstructions(false), message(""){
    buttons.push_back(Button("Start New Game", game->getFont(), 380.0f, 250.0f, 200.0f, 45.0f));
    buttons.push_back(Button("Continue", game->getFont(), 380.0f, 310.0f, 200.0f, 45.0f));
    buttons.push_back(Button("Instructions", game->getFont(), 380.0f, 370.0f, 200.0f, 45.0f));
    buttons.push_back(Button("Exit", game->getFont(), 380.0f, 430.0f, 200.0f, 45.0f));
    buttons[0].setColor(sf::Color(190, 140, 40));
    buttons[1].setEnabled(game->getSaveManager().hasSave());
}

void MenuState::handleInput(const sf::Event& event){
    if (showingInstructions){
        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::KeyPressed){
            showingInstructions = false;
        }
        return;
    }
    if (buttons[0].isClicked(event)){
        SoundManager::getInstance().playClick();
        game->showHeroSelect();
    }
    else if (buttons[1].isClicked(event)){
        SoundManager::getInstance().playClick();
        if (!game->continueGame()){
            message = "Save file could not be read.";
        }
    }
    else if (buttons[2].isClicked(event)){
        SoundManager::getInstance().playClick();
        showingInstructions = true;
    }
    else if (buttons[3].isClicked(event)){
        game->quit();
    }
}

void MenuState::update(float dt){
}

void MenuState::draw(sf::RenderWindow& window) const{
    sf::RectangleShape sky(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    sky.setFillColor(sf::Color(25, 35, 75));
    window.draw(sky);
    for (int i = 0; i < 6; i += 1){
        sf::RectangleShape tower(sf::Vector2f(110.0f, 120.0f + (i % 3) * 50.0f));
        tower.setFillColor(sf::Color(40, 55, 105));
        tower.setPosition(i * 165.0f, WINDOW_HEIGHT - tower.getSize().y);
        window.draw(tower);
    }
    drawCenteredText(window, "TEEN TALWAR", 56, 110.0f, sf::Color(240, 200, 80));
    drawCenteredText(window, "A story-driven arena adventure", 20, 185.0f, sf::Color(220, 220, 220));
    for (int i = 0; i < (int)buttons.size(); i += 1){
        buttons[i].draw(window);
    }
    if (message != ""){
        drawCenteredText(window, message, 16, 500.0f, sf::Color(240, 120, 120));
    }
    if (showingInstructions){
        drawPanel(window, 130.0f, 90.0f, 700.0f, 420.0f);
        drawCenteredText(window, "How to Play", 30, 110.0f, sf::Color(240, 200, 80));
        drawText(window, "Pick a hero: Knight, Mage or Rogue. Each has a special move.", 17, 160.0f, 170.0f);
        drawText(window, "Story: Space / Enter / click to continue, 1-2 or click for choices.", 17, 160.0f, 205.0f);
        drawText(window, "Minigames: WASD or arrow keys to move.", 17, 160.0f, 240.0f);
        drawText(window, "Shop: spend gold on weapons, armour and trinkets.", 17, 160.0f, 275.0f);
        drawText(window, "Arena: A/D or arrows move, W/Up jump, S/Down block.", 17, 160.0f, 310.0f);
        drawText(window, "J/Z quick attack, K/X power attack, L/C special. Jump the '!!' slams!", 17, 160.0f, 335.0f);
        drawText(window, "Win fights to earn gold, XP and crystal shards.", 17, 160.0f, 370.0f);
        drawText(window, "Level up to spend stat points. Collect all 4 shards!", 17, 160.0f, 405.0f);
        drawCenteredText(window, "Click or press any key to close", 15, 470.0f, sf::Color(180, 180, 180));
    }
}
