#include <iostream>
#include "StoryManager.hpp"
#include <fstream>
#include "NPC.hpp"

using namespace std;
// Game::startChapter()
//     
// storyManager.loadChapter(chapter)
//     
// clear() removes previous scenes and deletes old NPCs
//     
// Open assets/story/chapterN.txt
//     
// Read each entry:
//     TITLE   store chapter title
//     NPC    create and store an NPC
//     SCENE  create and store a StoryScene
//     LINE   add dialogue to the latest scene
//     CHOICE add a choice to the latest scene
//     
// sceneIndex = 0 and lineIndex = 0 (set by clear())
//     
// Return true if at least one scene was loaded
//     
// Game creates StoryState to display the story
StoryManager::StoryManager() : storyFolder("assets/story/"), chapterTitle(""), scenes(), npcs(), sceneIndex(0), lineIndex(0){
}

StoryManager::StoryManager(const string& storyFolder)
    : storyFolder(storyFolder), chapterTitle(""), scenes(), npcs(), sceneIndex(0), lineIndex(0){
}

StoryManager::~StoryManager(){
    clear();
}

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

const StoryScene& StoryManager::getCurrentScene() const{
    return scenes[sceneIndex];
}

const DialogueLine& StoryManager::getCurrentLine() const{
    return scenes[sceneIndex][lineIndex];
}

bool StoryManager::isLastLine() const{
    return lineIndex == scenes[sceneIndex].getLineCount() - 1;
}

bool StoryManager::nextLine(){
    if (isLastLine()){
        return false;
    }
    lineIndex += 1;
    return true;
}

bool StoryManager::nextScene(){
    if (sceneIndex + 1 >= (int)scenes.size()){
        return false;
    }
    sceneIndex += 1;
    lineIndex = 0;
    return true;
}
