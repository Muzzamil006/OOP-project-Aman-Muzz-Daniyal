#include <iostream>
#include "Choice.hpp"

using namespace std;

Choice::Choice() : text(""), reply(""), rewardGold(0){
}

Choice::Choice(const string& text, const string& reply, int rewardGold) : text(text), reply(reply), rewardGold(rewardGold){
}

const string& Choice::getText() const{
    return text;
}

const string& Choice::getReply() const{
    return reply;
}

int Choice::getRewardGold() const{
    return rewardGold;
}
