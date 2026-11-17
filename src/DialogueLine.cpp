#include <iostream>
#include "DialogueLine.hpp"

using namespace std;

DialogueLine::DialogueLine() : speaker("Narrator"), text(""){
}

DialogueLine::DialogueLine(const string& speaker, const string& text) : speaker(speaker), text(text){
}

const string& DialogueLine::getSpeaker() const{
    return speaker;
}

const string& DialogueLine::getText() const{
    return text;
}

bool DialogueLine::isHeroLine() const{
    return speaker == "HERO";
}
