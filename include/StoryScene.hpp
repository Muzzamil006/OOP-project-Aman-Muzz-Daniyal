#ifndef STORYSCENE_HPP
#define STORYSCENE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "DialogueLine.hpp"
#include "Choice.hpp"

using namespace std;

class NPC;

class StoryScene{
private:
    string title;
    sf::Color background;
    NPC* npc;
    vector<DialogueLine> lines;
    vector<Choice> choices;

public:
    StoryScene();
    StoryScene(const string& title, const sf::Color& background, NPC* npc);

    const string& getTitle() const;
    const sf::Color& getBackground() const;
    const NPC* getNpc() const;

    void addLine(const DialogueLine& line);
    void addLine(const string& speaker, const string& text);
    void addChoice(const Choice& choice);

    int getLineCount() const;
    const DialogueLine& operator[](int index) const;
    bool hasChoices() const;
    int getChoiceCount() const;
    const Choice& getChoice(int index) const;
};

#endif
