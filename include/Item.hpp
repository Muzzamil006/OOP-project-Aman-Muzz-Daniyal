#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Stats.hpp"

using namespace std;

class Item{
private:
    string name;
    int type;
    int price;
    Stats bonus;

public:
    static const int WEAPON = 0;
    static const int ARMOUR = 1;
    static const int TRINKET = 2;

    Item();
    Item(const string& name, int type, int price, const Stats& bonus);

    const string& getName() const;
    int getType() const;
    string getTypeName() const;
    int getPrice() const;
    const Stats& getBonus() const;
    string getBonusText() const;

    bool operator==(const Item& other) const;
    friend ostream& operator<<(ostream& out, const Item& item);

    static vector<Item> getShopCatalog(int chapter);
};

#endif
