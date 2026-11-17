#ifndef MINIGAMESTATE_HPP
#define MINIGAMESTATE_HPP

#include <iostream>
#include "GameState.hpp"
#include "Button.hpp"

using namespace std;

class Minigame;

class MinigameState : public GameState{
private:
    Minigame* minigame;
    int phase;
    Button startButton;
    Button retryButton;
    Button continueButton;

public:
    static const int PHASE_INTRO = 0;
    static const int PHASE_PLAYING = 1;
    static const int PHASE_RESULT = 2;
    static const int RETRY_COST = 5;

    MinigameState();
    MinigameState(Game* game, Minigame* minigame);
    ~MinigameState() override;
    MinigameState(const MinigameState& other) = delete;
    MinigameState& operator=(const MinigameState& other) = delete;

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
};

#endif
