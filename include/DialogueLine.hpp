#ifndef DIALOGUELINE_HPP
#define DIALOGUELINE_HPP

#include <iostream>
#include <string>

using namespace std;

class DialogueLine{
private:
    string speaker;
    string text;

public:
    DialogueLine();
    DialogueLine(const string& speaker, const string& text);

    const string& getSpeaker() const;
    const string& getText() const;
    bool isHeroLine() const;
};

#endif
