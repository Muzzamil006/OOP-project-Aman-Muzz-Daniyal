#include <iostream>
#include "Item.hpp"

using namespace std;

Item::Item() : name("Nothing"), type(TRINKET), price(0), bonus(){
}

Item::Item(const string& name, int type, int price, const Stats& bonus) : name(name), type(type), price(price), bonus(bonus){
}

const string& Item::getName() const{
    return name;
}

int Item::getType() const{
    return type;
}

string Item::getTypeName() const{
    if (type == WEAPON){
        return "Weapon";
    }
    if (type == ARMOUR){
        return "Armour";
    }
    return "Trinket";
}

int Item::getPrice() const{
    return price;
}

const Stats& Item::getBonus() const{
    return bonus;
}

string Item::getBonusText() const{
    string text = "";
    if (bonus.getStrength() > 0){
        text += "+" + to_string(bonus.getStrength()) + " STR ";
    }
    if (bonus.getDefence() > 0){
        text += "+" + to_string(bonus.getDefence()) + " DEF ";
    }
    if (bonus.getAgility() > 0){
        text += "+" + to_string(bonus.getAgility()) + " AGI ";
    }
    if (bonus.getVitality() > 0){
        text += "+" + to_string(bonus.getVitality()) + " VIT ";
    }
    return text;
}

bool Item::operator==(const Item& other) const{
    return name == other.name;
}

ostream& operator<<(ostream& out, const Item& item){
    out << item.type << " " << item.price << " " << item.bonus << " " << item.name;
    return out;
}

vector<Item> Item::getShopCatalog(int chapter){
    vector<Item> catalog;
    catalog.push_back(Item("Wooden Sword", WEAPON, 40, Stats(2, 0, 0, 0)));
    catalog.push_back(Item("Leather Armour", ARMOUR, 40, Stats(0, 2, 0, 0)));
    catalog.push_back(Item("Swift Sandals", TRINKET, 50, Stats(0, 0, 3, 0)));
    catalog.push_back(Item("Amulet of Life", TRINKET, 70, Stats(0, 0, 0, 3)));
    catalog.push_back(Item("Iron Sword", WEAPON, 90, Stats(4, 0, 0, 0)));
    catalog.push_back(Item("Chainmail", ARMOUR, 100, Stats(0, 4, 0, 1)));
    if (chapter >= 2){
        catalog.push_back(Item("Steel Greatsword", WEAPON, 160, Stats(7, 0, -1, 0)));
        catalog.push_back(Item("Frostproof Plate", ARMOUR, 170, Stats(0, 6, 0, 2)));
    }
    return catalog;
}
