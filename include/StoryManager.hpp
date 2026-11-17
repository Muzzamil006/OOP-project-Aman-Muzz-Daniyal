#ifndef STORYMANAGER_HPP
#define STORYMANAGER_HPP

#include <iostream>
#include <string>
#include <vector>
#include "StoryScene.hpp"

using namespace std;

class NPC;

class StoryManager{
private:
    string storyFolder;
    string chapterTitle;
    vector<StoryScene> scenes;
    vector<NPC*> npcs;
    int sceneIndex;
    int lineIndex;

    void clear();
    vector<string> split(const string& text, char separator) const;

public:
    StoryManager();
    StoryManager(const string& storyFolder);
    ~StoryManager();
    StoryManager(const StoryManager& other) = delete;
    StoryManager& operator=(const StoryManager& other) = delete;

    bool loadChapter(int chapter);
    const string& getChapterTitle() const;
    const StoryScene& getCurrentScene() const;
    const DialogueLine& getCurrentLine() const;
    bool isLastLine() const;
    bool nextLine();
    bool nextScene();
};

#endif
