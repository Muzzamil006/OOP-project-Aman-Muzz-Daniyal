#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <iostream>
#include <vector>
#include "GameState.hpp"
#include "Button.hpp"

using namespace std;

class MenuState : public GameState{
private:
    vector<Button> buttons;
    bool showingInstructions;
    string message;

public:
    MenuState();
    MenuState(Game* game);

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
};

#endif
