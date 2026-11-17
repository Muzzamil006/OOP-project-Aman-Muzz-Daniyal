#include <iostream>
#include "CharacterSprite.hpp"
#include <cmath>
#include <fstream>

using namespace std;

const float CharacterSprite::ATTACK_DURATION = 0.3f;
const float CharacterSprite::HURT_DURATION = 0.3f;

CharacterSprite::CharacterSprite()
    : texture(), hasTexture(false), armourTexture(), hasArmourTexture(false), showArmour(false), origin(0.0f, 0.0f),
      placeholderColor(sf::Color(170, 40, 40)), placeholderSize(20.0f, 40.0f), facingRight(true), scale(3.0f),
      animationTime(0.0f), attackTimer(0.0f), hurtTimer(0.0f), defeated(false){
}

CharacterSprite::CharacterSprite(const sf::Color& placeholderColor, bool facingRight, float scale)
    : texture(), hasTexture(false), armourTexture(), hasArmourTexture(false), showArmour(false), origin(0.0f, 0.0f),
      placeholderColor(placeholderColor), placeholderSize(20.0f, 40.0f), facingRight(facingRight), scale(scale),
      animationTime(0.0f), attackTimer(0.0f), hurtTimer(0.0f), defeated(false){
}

bool CharacterSprite::fileExists(const string& path) const{
    ifstream file(path);
    return file.good();
}

bool CharacterSprite::loadTexture(const string& path){
    if (!fileExists(path) || !texture.loadFromFile(path)){
        hasTexture = false;
        return false;
    }
    hasTexture = true;
    texture.setSmooth(false);
    origin = sf::Vector2f(texture.getSize().x / 2.0f, (float)texture.getSize().y);
    return true;
}

bool CharacterSprite::loadTexture(const string& path, float originX, float originY){
    if (!loadTexture(path)){
        return false;
    }
    origin = sf::Vector2f(originX, originY);
    return true;
}

bool CharacterSprite::loadArmour(const string& path){
    if (!fileExists(path) || !armourTexture.loadFromFile(path)){
        hasArmourTexture = false;
        return false;
    }
    hasArmourTexture = true;
    armourTexture.setSmooth(false);
    return true;
}

void CharacterSprite::setFacingRight(bool right){
    facingRight = right;
}

void CharacterSprite::setScale(float newScale){
    scale = newScale;
}

void CharacterSprite::setShowArmour(bool show){
    showArmour = show;
}

void CharacterSprite::setDefeated(bool isDefeated){
    defeated = isDefeated;
}

void CharacterSprite::setPlaceholderSize(float width, float height){
    placeholderSize = sf::Vector2f(width, height);
}

bool CharacterSprite::hasImage() const{
    return hasTexture;
}

float CharacterSprite::getScale() const{
    return scale;
}

float CharacterSprite::getHeight() const{
    if (hasTexture){
        return texture.getSize().y * scale;
    }
    return placeholderSize.y * scale;
}

void CharacterSprite::update(float dt){
    animationTime += dt;
    if (attackTimer > 0.0f){
        attackTimer -= dt;
    }
    if (hurtTimer > 0.0f){
        hurtTimer -= dt;
    }
}

void CharacterSprite::playAttack(){
    attackTimer = ATTACK_DURATION;
}

void CharacterSprite::playHurt(){
    hurtTimer = HURT_DURATION;
}

sf::Transform CharacterSprite::buildTransform(float footX, float footY, float finalScale) const{
    float direction = facingRight ? 1.0f : -1.0f;
    sf::Transform transform;
    if (defeated){
        transform.translate(footX, footY - 6.0f * finalScale);
        transform.rotate(-90.0f * direction);
    }
    else{
        float lunge = 0.0f;
        float tilt = 0.0f;
        if (attackTimer > 0.0f){
            float progress = 1.0f - attackTimer / ATTACK_DURATION;
            lunge = sin(progress * 3.14159f) * 6.0f * finalScale;
            tilt = sin(progress * 3.14159f) * 8.0f;
        }
        float bob = floor(sin(animationTime * 4.0f) + 0.5f) * finalScale;
        transform.translate(footX + lunge * direction, footY + bob);
        transform.rotate(tilt * direction);
    }
    transform.scale(finalScale * direction, finalScale);
    return transform;
}

void CharacterSprite::draw(sf::RenderTarget& target, float footX, float footY) const{
    draw(target, footX, footY, 1.0f);
}

void CharacterSprite::draw(sf::RenderTarget& target, float footX, float footY, float scaleMultiplier) const{
    float finalScale = scale * scaleMultiplier;
    sf::Color tint = hurtTimer > 0.0f ? sf::Color(255, 110, 110) : sf::Color::White;

    sf::CircleShape shadow(10.0f);
    shadow.setOrigin(10.0f, 10.0f);
    shadow.setScale(finalScale * (defeated ? 3.0f : 1.5f), finalScale * 0.3f);
    shadow.setPosition(footX, footY);
    shadow.setFillColor(sf::Color(0, 0, 0, 60));
    target.draw(shadow);

    sf::Transform transform = buildTransform(footX, footY, finalScale);
    if (hasTexture){
        sf::Sprite body(texture);
        body.setOrigin(origin);
        body.setColor(tint);
        target.draw(body, transform);
        if (showArmour && hasArmourTexture){
            sf::Sprite armour(armourTexture);
            armour.setOrigin(origin);
            armour.setColor(tint);
            target.draw(armour, transform);
        }
    }
    else{
        sf::RectangleShape placeholder(placeholderSize);
        placeholder.setOrigin(placeholderSize.x / 2.0f, placeholderSize.y);
        sf::Color fill = placeholderColor;
        if (hurtTimer > 0.0f){
            fill = sf::Color(255, 110, 110);
        }
        placeholder.setFillColor(fill);
        placeholder.setOutlineColor(sf::Color::Black);
        placeholder.setOutlineThickness(1.0f);
        target.draw(placeholder, transform);
    }
}
