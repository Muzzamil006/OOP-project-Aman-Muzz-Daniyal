#include <iostream>
#include "GameState.hpp"
#include "Game.hpp"

using namespace std;

GameState::GameState() : game(nullptr){
}

GameState::GameState(Game* game) : game(game){
}

GameState::~GameState(){
}

void GameState::drawText(sf::RenderWindow& window, const string& text, unsigned int size, float x, float y) const{
    drawText(window, text, size, x, y, sf::Color::White);
}

void GameState::drawText(sf::RenderWindow& window, const string& text, unsigned int size, float x, float y, const sf::Color& color) const{
    sf::Text drawable(text, game->getFont(), size);
    drawable.setFillColor(color);
    drawable.setPosition(x, y);
    window.draw(drawable);
}

void GameState::drawCenteredText(sf::RenderWindow& window, const string& text, unsigned int size, float y, const sf::Color& color) const{
    sf::Text drawable(text, game->getFont(), size);
    drawable.setFillColor(color);
    sf::FloatRect bounds = drawable.getLocalBounds();
    drawable.setPosition(WINDOW_WIDTH / 2.0f - bounds.width / 2.0f - bounds.left, y);
    window.draw(drawable);
}

void GameState::drawPanel(sf::RenderWindow& window, float x, float y, float width, float height) const{
    sf::RectangleShape panel(sf::Vector2f(width, height));
    panel.setPosition(x, y);
    panel.setFillColor(sf::Color(15, 15, 30, 220));
    panel.setOutlineColor(sf::Color(230, 190, 70));
    panel.setOutlineThickness(2.0f);
    window.draw(panel);
}
