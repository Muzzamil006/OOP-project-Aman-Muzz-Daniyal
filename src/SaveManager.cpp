#include <iostream>
#include "SaveManager.hpp"
#include <fstream>
#include "Hero.hpp"

using namespace std;

SaveManager::SaveManager() : filePath("save.txt"){
}

SaveManager::SaveManager(const string& filePath) : filePath(filePath){
}

const string& SaveManager::getFilePath() const{
    return filePath;
}

bool SaveManager::hasSave() const{
    ifstream file(filePath);
    return file.good();
}

bool SaveManager::save(const Hero& hero, int chapter) const{
    ofstream file(filePath);
    if (!file){
        return false;
    }
    file << hero.getHeroType() << " " << chapter << "\n";
    file << hero.getLevel() << " " << hero.getExperience() << " " << hero.getGold() << " " << hero.getStatPoints() << " " << hero.getShards() << "\n";
    file << hero.getBaseStats() << "\n";
    const Inventory& inventory = hero.getInventory();
    file << inventory.getSize() << "\n";
    for (int i = 0; i < inventory.getSize(); i += 1){
        file << inventory[i] << "\n";
    }
    return true;
}

Hero* SaveManager::load(int& chapter) const{
    ifstream file(filePath);
    if (!file){
        return nullptr;
    }
    int heroType = 0;
    int level = 1;
    int experience = 0;
    int gold = 0;
    int statPoints = 0;
    int shards = 0;
    int strength = 0;
    int defence = 0;
    int agility = 0;
    int vitality = 0;
    int itemCount = 0;
    if (!(file >> heroType >> chapter >> level >> experience >> gold >> statPoints >> shards >> strength >> defence >> agility >> vitality >> itemCount)){
        return nullptr;
    }
    Hero* hero = Hero::create(heroType);
    hero->setBaseStats(Stats(strength, defence, agility, vitality));
    hero->setProgress(level, experience, gold, statPoints, shards);
    for (int i = 0; i < itemCount; i += 1){
        int type = 0;
        int price = 0;
        string name;
        file >> type >> price >> strength >> defence >> agility >> vitality;
        getline(file, name);
        if (name.size() > 0 && name[0] == ' '){
            name = name.substr(1);
        }
        hero->getInventory().addItem(name, type, price, Stats(strength, defence, agility, vitality));
    }
    hero->refreshAppearance();
    hero->restoreFullHealth();
    return hero;
}
