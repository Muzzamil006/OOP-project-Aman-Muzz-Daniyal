#include <iostream>
#include "HeroSelectState.hpp"
#include "Game.hpp"
#include "Hero.hpp"
#include "SoundManager.hpp"

using namespace std;

HeroSelectState::HeroSelectState() : GameState(), previews(), cardAreas(), selectedIndex(0), confirmButton(), backButton(){
}

HeroSelectState::HeroSelectState(Game* game)
    : GameState(game), previews(), cardAreas(), selectedIndex(0),
      confirmButton("Confirm", game->getFont(), 500.0f, 520.0f, 160.0f, 45.0f),
      backButton("Back", game->getFont(), 300.0f, 520.0f, 160.0f, 45.0f){
    confirmButton.setColor(sf::Color(190, 140, 40));
    for (int i = 0; i < 3; i += 1){
        previews.push_back(Hero::create(i));
        cardAreas.push_back(sf::FloatRect(70.0f + i * 285.0f, 110.0f, 250.0f, 380.0f));
    }
}

HeroSelectState::~HeroSelectState(){
    for (int i = 0; i < (int)previews.size(); i += 1){
        delete previews[i];
    }
}

void HeroSelectState::handleInput(const sf::Event& event){
    if (event.type == sf::Event::MouseButtonPressed){
        for (int i = 0; i < (int)cardAreas.size(); i += 1){
            if (cardAreas[i].contains((float)event.mouseButton.x, (float)event.mouseButton.y)){
                selectedIndex = i;
                SoundManager::getInstance().playClick();
            }
        }
    }
    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::Left && selectedIndex > 0){
            selectedIndex -= 1;
        }
        else if (event.key.code == sf::Keyboard::Right && selectedIndex < 2){
            selectedIndex += 1;
        }
        else if (event.key.code == sf::Keyboard::Enter){
            game->startNewGame(previews[selectedIndex]->getHeroType());
        }
    }
    if (confirmButton.isClicked(event)){
        SoundManager::getInstance().playClick();
        game->startNewGame(previews[selectedIndex]->getHeroType());
    }
    else if (backButton.isClicked(event)){
        game->showMenu();
    }
}

void HeroSelectState::update(float dt){
    for (int i = 0; i < (int)previews.size(); i += 1){
        previews[i]->update(dt);
    }
}

void HeroSelectState::draw(sf::RenderWindow& window) const{
    drawCenteredText(window, "Choose Your Hero", 36, 40.0f, sf::Color(240, 200, 80));
    for (int i = 0; i < (int)previews.size(); i += 1){
        const Hero* hero = previews[i];
        sf::FloatRect area = cardAreas[i];
        sf::RectangleShape card(sf::Vector2f(area.width, area.height));
        card.setPosition(area.left, area.top);
        card.setFillColor(sf::Color(30, 30, 55));
        card.setOutlineThickness(i == selectedIndex ? 4.0f : 1.0f);
        card.setOutlineColor(i == selectedIndex ? sf::Color(240, 200, 80) : sf::Color(120, 120, 150));
        window.draw(card);

        hero->getSprite().draw(window, area.left + area.width / 2.0f - 20.0f, area.top + 160.0f, 0.75f);

        Stats stats = hero->getTotalStats();
        float textX = area.left + 25.0f;
        drawText(window, hero->getName(), 24, textX, area.top + 168.0f, sf::Color(240, 200, 80));
        drawText(window, hero->getTitle(), 18, textX, area.top + 198.0f);
        drawText(window, "HP: " + to_string(hero->getMaxHealth()), 16, textX, area.top + 226.0f);
        drawText(window, "STR " + to_string(stats.getStrength()) + "   DEF " + to_string(stats.getDefence()), 16, textX, area.top + 250.0f);
        drawText(window, "AGI " + to_string(stats.getAgility()) + "   VIT " + to_string(stats.getVitality()), 16, textX, area.top + 274.0f);
        drawText(window, "Special:", 16, textX, area.top + 304.0f, sf::Color(180, 180, 180));
        drawText(window, hero->getSpecialName(), 18, textX, area.top + 326.0f, sf::Color(130, 200, 255));
    }
    confirmButton.draw(window);
    backButton.draw(window);
}
