#include <iostream>
#include "ShopState.hpp"
#include "Game.hpp"
#include "Hero.hpp"
#include "SoundManager.hpp"

using namespace std;

ShopState::ShopState() : GameState(), catalog(), buyButtons(), leaveButton(), message(""){
}

ShopState::ShopState(Game* game)
    : GameState(game), catalog(Item::getShopCatalog(game->getChapter())), buyButtons(),
      leaveButton("Enter the Arena", game->getFont(), 690.0f, 520.0f, 220.0f, 50.0f),
      message("Welcome to the armoury, traveller."){
    leaveButton.setColor(sf::Color(170, 50, 50));
    for (int i = 0; i < (int)catalog.size(); i += 1){
        buyButtons.push_back(Button("Buy", game->getFont(), 520.0f, 112.0f + i * 52.0f, 90.0f, 38.0f));
    }
    refreshButtons();
}

void ShopState::refreshButtons(){
    const Hero* hero = game->getHero();
    for (int i = 0; i < (int)catalog.size(); i += 1){
        if (hero->getInventory().contains(catalog[i])){
            buyButtons[i].setText("Owned");
            buyButtons[i].setEnabled(false);
        }
        else{
            buyButtons[i].setText("Buy");
            buyButtons[i].setEnabled(hero->getGold() >= catalog[i].getPrice());
        }
    }
}

void ShopState::handleInput(const sf::Event& event){
    Hero* hero = game->getHero();
    for (int i = 0; i < (int)buyButtons.size(); i += 1){
        if (buyButtons[i].isClicked(event)){
            if (hero->spendGold(catalog[i].getPrice())){
                hero->getInventory() += catalog[i];
                hero->recalculateMaxHealth();
                hero->refreshAppearance();
                SoundManager::getInstance().playClick();
                message = "You bought the " + catalog[i].getName() + ".";
            }
            refreshButtons();
        }
    }
    if (leaveButton.isClicked(event)){
        game->startArena();
    }
}

void ShopState::update(float dt){
    game->getHero()->update(dt);
}

void ShopState::draw(sf::RenderWindow& window) const{
    const Hero* hero = game->getHero();
    drawText(window, "The Armoury", 34, 40.0f, 25.0f, sf::Color(240, 200, 80));
    drawText(window, message, 16, 40.0f, 72.0f, sf::Color(200, 200, 200));
    for (int i = 0; i < (int)catalog.size(); i += 1){
        float y = 112.0f + i * 52.0f;
        drawPanel(window, 40.0f, y, 590.0f, 38.0f);
        drawText(window, catalog[i].getName(), 17, 52.0f, y + 8.0f);
        drawText(window, catalog[i].getTypeName(), 14, 225.0f, y + 10.0f, sf::Color(170, 170, 170));
        drawText(window, catalog[i].getBonusText(), 14, 305.0f, y + 10.0f, sf::Color(130, 220, 130));
        drawText(window, to_string(catalog[i].getPrice()) + "g", 16, 455.0f, y + 9.0f, sf::Color(240, 200, 80));
        buyButtons[i].draw(window);
    }
    Stats total = hero->getTotalStats();
    drawPanel(window, 660.0f, 112.0f, 260.0f, 280.0f);
    drawText(window, hero->getName() + " the " + hero->getTitle(), 18, 675.0f, 125.0f, sf::Color(240, 200, 80));
    drawText(window, "Level " + to_string(hero->getLevel()), 16, 675.0f, 160.0f);
    drawText(window, "Gold: " + to_string(hero->getGold()), 16, 675.0f, 190.0f, sf::Color(240, 200, 80));
    drawText(window, "HP: " + to_string(hero->getMaxHealth()), 16, 675.0f, 220.0f);
    drawText(window, "Strength: " + to_string(total.getStrength()), 16, 675.0f, 250.0f);
    drawText(window, "Defence: " + to_string(total.getDefence()), 16, 675.0f, 280.0f);
    drawText(window, "Agility: " + to_string(total.getAgility()), 16, 675.0f, 310.0f);
    drawText(window, "Vitality: " + to_string(total.getVitality()), 16, 675.0f, 340.0f);
    drawText(window, "Items owned: " + to_string(hero->getInventory().getSize()), 14, 675.0f, 368.0f, sf::Color(170, 170, 170));
    hero->getSprite().draw(window, 840.0f, 365.0f, 0.75f);
    leaveButton.draw(window);
}
