#include <iostream>
#include "StoryManager.hpp"
#include <fstream>
#include "NPC.hpp"

using namespace std;

StoryManager::StoryManager() : storyFolder("assets/story/"), chapterTitle(""), scenes(), npcs(), sceneIndex(0), lineIndex(0){
}

StoryManager::StoryManager(const string& storyFolder)
    : storyFolder(storyFolder), chapterTitle(""), scenes(), npcs(), sceneIndex(0), lineIndex(0){
}

StoryManager::~StoryManager(){
    clear();
}

/*
Called before loading a chapter and when the manager is destroyed
Delete the NPC objects owned by this manager
Clear the NPC and scene vectors
Clear the chapter title
Reset sceneIndex and lineIndex to zero
*/
void StoryManager::clear(){
    for (int i = 0; i < (int)npcs.size(); i += 1){
        delete npcs[i];
    }
    npcs.clear();
    scenes.clear();
    chapterTitle = "";
    sceneIndex = 0;
    lineIndex = 0;
}

vector<string> StoryManager::split(const string& text, char separator) const{
    vector<string> parts;
    string current = "";
    for (int i = 0; i < (int)text.size(); i += 1){
        if (text[i] == separator){
            parts.push_back(current);
            current = "";
        }
        else{
            current += text[i];
        }
    }
    parts.push_back(current);
    return parts;
}

/*
Game::startChapter() calls storyManager.loadChapter(chapter)
clear() removes previous story data and resets both indices to zero
Open assets/story/chapterN.txt
Return false if the file cannot be opened
Read each entry:
    TITLE: store the chapter title
    NPC: create and store an NPC
    SCENE: create and store a StoryScene
    LINE: add dialogue to the latest scene
    CHOICE: add a choice to the latest scene
Return true if at least one scene was loaded
After successful loading, Game creates StoryState to display the story
*/
bool StoryManager::loadChapter(int chapter){
    clear();
    ifstream file(storyFolder + "chapter" + to_string(chapter) + ".txt");
    if (!file){
        return false;
    }
    string line;
    while (getline(file, line)){
        if (line.size() > 0 && line[line.size() - 1] == '\r'){
            line = line.substr(0, line.size() - 1);
        }
        int space = (int)line.find(' ');
        if (space < 0){
            continue;
        }
        string keyword = line.substr(0, space);
        vector<string> parts = split(line.substr(space + 1), ';');
        if (keyword == "TITLE"){
            chapterTitle = parts[0];
        }
        else if (keyword == "NPC" && parts.size() >= 5){
            npcs.push_back(new NPC(parts[0], parts[1], sf::Color(stoi(parts[2]), stoi(parts[3]), stoi(parts[4]))));
        }
        else if (keyword == "SCENE" && parts.size() >= 5){
            int npcIndex = stoi(parts[1]);
            NPC* sceneNpc = nullptr;
            if (npcIndex >= 0 && npcIndex < (int)npcs.size()){
                sceneNpc = npcs[npcIndex];
            }
            scenes.push_back(StoryScene(parts[0], sf::Color(stoi(parts[2]), stoi(parts[3]), stoi(parts[4])), sceneNpc));
        }
        else if (keyword == "LINE" && parts.size() >= 2 && scenes.size() > 0){
            scenes[scenes.size() - 1].addLine(parts[0], parts[1]);
        }
        else if (keyword == "CHOICE" && parts.size() >= 3 && scenes.size() > 0){
            scenes[scenes.size() - 1].addChoice(Choice(parts[0], parts[1], stoi(parts[2])));
        }
    }
    return scenes.size() > 0;
}

const string& StoryManager::getChapterTitle() const{
    return chapterTitle;
}

/*
StoryState requests the active scene for drawing or building choice buttons
Use sceneIndex to select scenes[sceneIndex]
Return the existing scene by const reference
After nextScene() changes the index, this returns the new scene
*/
const StoryScene& StoryManager::getCurrentScene() const{
    return scenes[sceneIndex];
}

/*
StoryState::draw() requests the active dialogue line
Select scenes[sceneIndex]
Use StoryScene::operator[] with lineIndex to select its dialogue line
Return that line by const reference
StoryState displays the speaker and text
*/
const DialogueLine& StoryManager::getCurrentLine() const{
    return scenes[sceneIndex][lineIndex];
}

/*
Compare lineIndex with the final dialogue index in the current scene
Return true when lineIndex equals getLineCount() minus one
nextLine() uses this to decide whether it can advance
StoryState uses this to decide whether choices should appear
*/
bool StoryManager::isLastLine() const{
    return lineIndex == scenes[sceneIndex].getLineCount() - 1;
}

/*
StoryState::advance() requests the next dialogue line
Check isLastLine()
If already at the last line, return false
StoryState then calls goToNextScene()
Otherwise, increase lineIndex by one and return true
On the next frame, StoryState::draw() displays the newly selected line
*/
bool StoryManager::nextLine(){
    if (isLastLine()){
        return false;
    }
    lineIndex += 1;
    return true;
}

/*
StoryState::goToNextScene() requests the next scene
Check whether another scene exists
If no scene remains, return false and StoryState asks Game to start the minigame
Otherwise, increase sceneIndex by one
Reset lineIndex to zero for the new scene
Return true so StoryState rebuilds its choice buttons
The next draw displays the new scene's first dialogue line
*/
bool StoryManager::nextScene(){
    if (sceneIndex + 1 >= (int)scenes.size()){
        return false;
    }
    sceneIndex += 1;
    lineIndex = 0;
    return true;
}
