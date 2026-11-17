#include <iostream>
#include "Character.hpp"
using namespace std;

int Character::characterCount = 0;
const float Character::GROUND_Y = 400.0;

Character::Character() : name("Unknown"), baseStats(1, 1, 1, 1), health(1), maxHealth(1), positionX(0.0), heightAboveGround(0.0), color(sf::Color::White), sprite(){
    characterCount += 1;
    recalculateMaxHealth();
    health = maxHealth;
}
Character::Character(const string& name, const Stats& baseStats, float positionX, const sf::Color& color): name(name), baseStats(baseStats), health(1), maxHealth(1), positionX(positionX), heightAboveGround(0.0), color(color),sprite(color, true, 3.0){
    characterCount += 1;
    recalculateMaxHealth();
    health = maxHealth;
}
Character::~Character(){
    characterCount -= 1;
}
const string& Character::getName() const{
    return name;
}
const Stats& Character::getBaseStats() const{
    return baseStats;
}
Stats Character::getTotalStats() const{
    return baseStats;
}
int Character::getHealth() const{
    return health;
}
int Character::getMaxHealth() const{
    return maxHealth;
}
float Character::getPositionX() const{
    return positionX;
}
const sf::Color& Character::getColor() const{
    return color;
}
float Character::getCenterX() const{
    return positionX + 30.0; // Character width 60 so middle 30
}
float Character::getHeightAboveGround() const{
    return heightAboveGround;
}
// no jump it will be 0 when jump greater 
void Character::setHeightAboveGround(float height){
    heightAboveGround = height;
}
CharacterSprite& Character::getSprite(){
    return sprite;
}

const CharacterSprite& Character::getSprite() const{
    return sprite;
}
void Character::update(float dt){
    sprite.update(dt);
}
bool Character::isAlive() const{
    return health > 0;
}
void Character::setBaseStats(const Stats& stats){
    baseStats = stats;
    recalculateMaxHealth();
}
void Character::setPositionX(float x){
    positionX = x;
}
void Character::moveBy(float amount){
    positionX += amount;
}
void Character::upgradeStat(int statIndex){
    baseStats.increase(statIndex);
    recalculateMaxHealth();
}
void Character::takeDamage(int amount){
    health -= amount;
    sprite.playHurt();
    if (health <= 0){
        health = 0;
        sprite.setDefeated(true);
    }
}
void Character::takeDamage(int amount, float multiplier){
    takeDamage((int)(amount * multiplier));
}
void Character::heal(int amount){
    sprite.setDefeated(false);
    health += amount;
    if (health > maxHealth){
        health = maxHealth;
    }
}
void Character::restoreFullHealth(){
    recalculateMaxHealth();
    health = maxHealth;
    sprite.setDefeated(false);
}
void Character::recalculateMaxHealth(){
    maxHealth = 50 + getTotalStats().getVitality() * 10;
    if (health > maxHealth){
        health = maxHealth;
    }
}
void Character::drawLabel(sf::RenderWindow& window, const sf::Font& font, const sf::Color& labelColor) const{
    sf::Text label(name, font, 16);
    label.setFillColor(labelColor);
    label.setOutlineColor(sf::Color(20, 10, 5));
    label.setOutlineThickness(2.0);
    sf::FloatRect bounds = label.getLocalBounds();
    label.setPosition(getCenterX() - bounds.width / 2.0, GROUND_Y + 12.0);
    window.draw(label);
}
void Character::draw(sf::RenderWindow& window, const sf::Font& font) const{
    sprite.draw(window, getCenterX(), GROUND_Y - heightAboveGround);
    drawLabel(window, font, sf::Color::White);
}
int Character::getCharacterCount(){
    return characterCount;
}
