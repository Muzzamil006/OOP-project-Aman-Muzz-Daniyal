#include <iostream>
#include "StoryScene.hpp"
#include "NPC.hpp"

using namespace std;
// StoryManager reads a SCENE entry
//     
// Extract title, NPC index and background RGB values
//     
// Find the matching NPC pointer, if valid
//     
// Create StoryScene(title, background, sceneNpc)
//     
// Constructor stores title, background and NPC pointer
//     
// Dialogue and choice vectors start empty
//     
// Store the scene in StoryManager's scenes vector
StoryScene::StoryScene() : title(""), background(sf::Color(30, 60, 40)), npc(nullptr), lines(), choices(){
}

StoryScene::StoryScene(const string& title, const sf::Color& background, NPC* npc)
    : title(title), background(background), npc(npc), lines(), choices(){
}

const string& StoryScene::getTitle() const{
    return title;
}

const sf::Color& StoryScene::getBackground() const{
    return background;
}

const NPC* StoryScene::getNpc() const{
    return npc;
}

void StoryScene::addLine(const DialogueLine& line){
    lines.push_back(line);
}

void StoryScene::addLine(const string& speaker, const string& text){
    addLine(DialogueLine(speaker, text));
}

void StoryScene::addChoice(const Choice& choice){
    choices.push_back(choice);
}

int StoryScene::getLineCount() const{
    return (int)lines.size();
}

const DialogueLine& StoryScene::operator[](int index) const{
    return lines[index];
}

bool StoryScene::hasChoices() const{
    return choices.size() > 0;
}

int StoryScene::getChoiceCount() const{
    return (int)choices.size();
}

const Choice& StoryScene::getChoice(int index) const{
    return choices[index];
}
