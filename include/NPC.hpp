#ifndef NPC_HPP
#define NPC_HPP

#include <iostream>
#include "Character.hpp"

using namespace std;

class NPC : public Character{
private:
    string role;

public:
    NPC();
    NPC(const string& name, const string& role, const sf::Color& color);

    const string& getRole() const;
    string getTitle() const override;
};

#endif
