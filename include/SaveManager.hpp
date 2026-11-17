#ifndef SAVEMANAGER_HPP
#define SAVEMANAGER_HPP

#include <iostream>
#include <string>

using namespace std;

class Hero;

class SaveManager{
private:
    string filePath;

public:
    SaveManager();
    SaveManager(const string& filePath);

    const string& getFilePath() const;
    bool hasSave() const;
    bool save(const Hero& hero, int chapter) const;
    Hero* load(int& chapter) const;
};

#endif
