#ifndef LEVELUPSTATE_HPP
#define LEVELUPSTATE_HPP

#include <iostream>
#include <vector>
#include "GameState.hpp"
#include "Button.hpp"

using namespace std;

class LevelUpState : public GameState{
private:
    vector<Button> plusButtons;
    Button continueButton;

public:
    LevelUpState();
    LevelUpState(Game* game);

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
};

#endif
