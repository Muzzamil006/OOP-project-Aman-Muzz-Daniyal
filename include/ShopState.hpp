#ifndef SHOPSTATE_HPP
#define SHOPSTATE_HPP

#include <iostream>
#include <vector>
#include <string>
#include "GameState.hpp"
#include "Button.hpp"
#include "Item.hpp"

using namespace std;

class ShopState : public GameState{
private:
    vector<Item> catalog;
    vector<Button> buyButtons;
    Button leaveButton;
    string message;

    void refreshButtons();

public:
    ShopState();
    ShopState(Game* game);

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
};

#endif
