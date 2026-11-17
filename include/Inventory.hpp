#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <iostream>
#include <vector>
#include "Item.hpp"

using namespace std;

class Inventory{
private:
    vector<Item> items;

public:
    Inventory();
    Inventory(const vector<Item>& items);

    void addItem(const Item& item);
    void addItem(const string& name, int type, int price, const Stats& bonus);
    Inventory& operator+=(const Item& item);
    const Item& operator[](int index) const;

    bool contains(const Item& item) const;
    int getSize() const;
    Stats getTotalBonus() const;
    void clear();
};

#endif
