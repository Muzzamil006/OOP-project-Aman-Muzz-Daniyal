#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include <iostream>
#include <string>
#include "GameState.hpp"
#include "Button.hpp"

using namespace std;

class GameOverState : public GameState{
private:
    string message;
    bool canRetry;
    Button retryButton;
    Button menuButton;

public:
    GameOverState();
    GameOverState(Game* game, const string& message, bool canRetry);

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
};

#endif
