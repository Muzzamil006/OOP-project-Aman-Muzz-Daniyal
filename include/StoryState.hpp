#ifndef STORYSTATE_HPP
#define STORYSTATE_HPP

#include <iostream>
#include <vector>
#include <string>
#include "GameState.hpp"
#include "Button.hpp"

using namespace std;

class StoryState : public GameState{
private:
    vector<Button> choiceButtons;
    bool showingReply;
    string replyText;

    void advance();
    void goToNextScene();
    void choose(int index);
    bool choicesVisible() const;
    void buildChoiceButtons();

public:
    StoryState();
    StoryState(Game* game);

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
};

#endif
