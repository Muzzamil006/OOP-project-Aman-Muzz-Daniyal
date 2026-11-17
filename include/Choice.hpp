#ifndef CHOICE_HPP
#define CHOICE_HPP

#include <iostream>
#include <string>

using namespace std;

class Choice{
private:
    string text;
    string reply;
    int rewardGold;

public:
    Choice();
    Choice(const string& text, const string& reply, int rewardGold);

    const string& getText() const;
    const string& getReply() const;
    int getRewardGold() const;
};

#endif
