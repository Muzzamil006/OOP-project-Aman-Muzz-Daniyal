#ifndef HEROSELECTSTATE_HPP
#define HEROSELECTSTATE_HPP

#include <iostream>
#include <vector>
#include "GameState.hpp"
#include "Button.hpp"

using namespace std;

class Hero;

class HeroSelectState : public GameState{
private:
    vector<Hero*> previews;
    vector<sf::FloatRect> cardAreas;
    int selectedIndex;
    Button confirmButton;
    Button backButton;

public:
    HeroSelectState();
    HeroSelectState(Game* game);
    ~HeroSelectState() override;
    HeroSelectState(const HeroSelectState& other) = delete;
    HeroSelectState& operator=(const HeroSelectState& other) = delete;

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
};

#endif
