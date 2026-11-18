#include <iostream>
#include "StoryScene.hpp"
#include "NPC.hpp"

using namespace std;

StoryScene::StoryScene() : title(""), background(sf::Color(30, 60, 40)), npc(nullptr), lines(), choices(){
}

/*
StoryManager reads a SCENE entry
Extract title, NPC index and background RGB values
Find the matching NPC pointer, if valid
Create StoryScene(title, background, sceneNpc)
Constructor stores title, background and NPC pointer
Dialogue and choice vectors start empty
Store the scene in StoryManager's scenes vector
*/
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

/*
Receive a DialogueLine object
Append it to this scene's lines vector
The dialogue is now stored for later display
*/
void StoryScene::addLine(const DialogueLine& line){
    lines.push_back(line);
}

/*
StoryManager reads a LINE entry from the chapter file
Split the entry into speaker and text
Call this overload on the most recently created scene
Create a DialogueLine object from the speaker and text
Call addLine(DialogueLine) to store it in the lines vector
*/
void StoryScene::addLine(const string& speaker, const string& text){
    addLine(DialogueLine(speaker, text));
}

/*
StoryManager reads a CHOICE entry from the chapter file
Extract option text, reply and gold reward
Convert the reward text to an integer using stoi()
Create a Choice object and pass it to this function
Append the choice to this scene's choices vector
*/
void StoryScene::addChoice(const Choice& choice){
    choices.push_back(choice);
}

int StoryScene::getLineCount() const{
    return (int)lines.size();
}

/*
StoryManager::getCurrentLine() evaluates scenes[sceneIndex][lineIndex]
The second pair of brackets calls this operator with lineIndex
Return lines[index] by const reference
StoryState uses that dialogue line's speaker and text when drawing
*/
const DialogueLine& StoryScene::operator[](int index) const{
    return lines[index];
}

/*
StoryState::choicesVisible() checks the current scene
Return whether this scene contains any choices
Choices appear only on the last dialogue line when no reply is showing
*/
bool StoryScene::hasChoices() const{
    return choices.size() > 0;
}

/*
StoryState::buildChoiceButtons() asks how many choices this scene has
Return the size of the choices vector
StoryState creates one button for each choice
*/
int StoryScene::getChoiceCount() const{
    return (int)choices.size();
}

/*
StoryState requests a choice by index
Return the stored Choice by const reference
When building buttons, StoryState reads its option text
When selecting a choice, StoryState reads its reply and gold reward
*/
const Choice& StoryScene::getChoice(int index) const{
    return choices[index];
}
