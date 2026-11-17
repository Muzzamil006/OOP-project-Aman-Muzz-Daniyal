#ifndef CHARACTERSPRITE_HPP
#define CHARACTERSPRITE_HPP

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class CharacterSprite{
private:
    sf::Texture texture;
    bool hasTexture;
    sf::Texture armourTexture;
    bool hasArmourTexture;
    bool showArmour;
    sf::Vector2f origin;
    sf::Color placeholderColor;
    sf::Vector2f placeholderSize;
    bool facingRight;
    float scale;
    float animationTime;
    float attackTimer;
    float hurtTimer;
    bool defeated;

    static const float ATTACK_DURATION;
    static const float HURT_DURATION;

    bool fileExists(const string& path) const;
    sf::Transform buildTransform(float footX, float footY, float finalScale) const;

public:
    CharacterSprite();
    CharacterSprite(const sf::Color& placeholderColor, bool facingRight, float scale);

    bool loadTexture(const string& path);
    bool loadTexture(const string& path, float originX, float originY);
    bool loadArmour(const string& path);

    void setFacingRight(bool right);
    void setScale(float newScale);
    void setShowArmour(bool show);
    void setDefeated(bool isDefeated);
    void setPlaceholderSize(float width, float height);
    bool hasImage() const;
    float getScale() const;
    float getHeight() const;

    void update(float dt);
    void playAttack();
    void playHurt();

    void draw(sf::RenderTarget& target, float footX, float footY) const;
    void draw(sf::RenderTarget& target, float footX, float footY, float scaleMultiplier) const;
};

#endif
