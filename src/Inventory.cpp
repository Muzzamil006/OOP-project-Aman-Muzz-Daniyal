#include <iostream>
#include "Inventory.hpp"

using namespace std;

Inventory::Inventory() : items(){
}

Inventory::Inventory(const vector<Item>& items) : items(items){
}

void Inventory::addItem(const Item& item){
    if (!contains(item)){
        items.push_back(item);
    }
}

void Inventory::addItem(const string& name, int type, int price, const Stats& bonus){
    addItem(Item(name, type, price, bonus));
}

Inventory& Inventory::operator+=(const Item& item){
    addItem(item);
    return *this;
}

const Item& Inventory::operator[](int index) const{
    return items[index];
}

bool Inventory::contains(const Item& item) const{
    for (int i = 0; i < (int)items.size(); i += 1){
        if (items[i] == item){
            return true;
        }
    }
    return false;
}

int Inventory::getSize() const{
    return (int)items.size();
}

Stats Inventory::getTotalBonus() const{
    Stats total;
    for (int i = 0; i < (int)items.size(); i += 1){
        total += items[i].getBonus();
    }
    return total;
}

void Inventory::clear(){
    items.clear();
}
