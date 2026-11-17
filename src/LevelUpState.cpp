#include <iostream>
#include "LevelUpState.hpp"
#include "Game.hpp"
#include "Hero.hpp"
#include "SoundManager.hpp"
using namespace std;

LevelUpState::LevelUpState() : GameState(), plusButtons(), continueButton(){
}
LevelUpState::LevelUpState(Game* game)
    : GameState(game), plusButtons(), continueButton("Continue", game->getFont(), 380.0, 480.0, 200.0, 45.0){
    continueButton.setColor(sf::Color(190, 140, 40));
    for (int i = 0; i < 4; i += 1){
        plusButtons[i] = Button("+", game->getFont(), 600.0, 220.0 + i * 55.0, 45.0, 40.0);
    }
}
void LevelUpState::handleInput(const sf::Event& event){
    Hero* hero = game->getHero();
    for (int i = 0; i < 4; i += 1){
        if (plusButtons[i].isClicked(event) && hero->spendStatPoint(i)){
            SoundManager::getInstance().playClick();
        }
    }
    if (continueButton.isClicked(event)){
        game->finishChapter();
    }
}
void LevelUpState::update(float dt){
    bool hasPoints = game->getHero()->getStatPoints() > 0;
    for (int i = 0; i < 4; i += 1){
        plusButtons[i].setEnabled(hasPoints);
    }
}
void LevelUpState::draw(sf::RenderWindow& window) const{
    const Hero* hero = game->getHero();
    Stats base = hero->getBaseStats();
    Stats total = hero->getTotalStats();
    int baseValues[4] = {base.getStrength(), base.getDefence(), base.getAgility(), base.getVitality()};
    int totalValues[4] = {total.getStrength(), total.getDefence(), total.getAgility(), total.getVitality()};
    string names[4] = {"Strength", "Defence", "Agility", "Vitality"};
    string effects[4] = {"more damage", "less damage taken", "faster movement", "+10 max HP each"};

    drawCenteredText(window, "LEVEL UP!", 46, 50.0, sf::Color(240, 200, 80));
    drawCenteredText(window, hero->getName() + " is now level " + to_string(hero->getLevel()), 20, 115.0, sf::Color::White);
    drawCenteredText(window, "Stat points to spend: " + to_string(hero->getStatPoints()), 20, 155.0, sf::Color(130, 220, 130));
    for (int i = 0; i < 4; i += 1){
        float y = 220.0 + i * 55.0;
        drawPanel(window, 250.0, y, 330.0, 40.0);
        drawText(window, names[i], 18, 265.0, y + 8.0);
        string value = to_string(baseValues[i]);
        if (totalValues[i] != baseValues[i]){
            value += "  (" + to_string(totalValues[i]) + ")";
        }
        drawText(window, value, 18, 390.0, y + 8.0, sf::Color(240, 200, 80));
        drawText(window, effects[i], 13, 665.0, y + 12.0, sf::Color(170, 170, 170));
        plusButtons[i].draw(window);
    }
    continueButton.draw(window);
}