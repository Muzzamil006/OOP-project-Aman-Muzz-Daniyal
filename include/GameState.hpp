#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class Game;

class GameState{
protected:
    Game* game;

    void drawText(sf::RenderWindow& window, const string& text, unsigned int size, float x, float y) const;
    void drawText(sf::RenderWindow& window, const string& text, unsigned int size, float x, float y, const sf::Color& color) const;
    void drawCenteredText(sf::RenderWindow& window, const string& text, unsigned int size, float y, const sf::Color& color) const;
    void drawPanel(sf::RenderWindow& window, float x, float y, float width, float height) const;

public:
    GameState();
    GameState(Game* game);
    virtual ~GameState();

    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};

#endif
