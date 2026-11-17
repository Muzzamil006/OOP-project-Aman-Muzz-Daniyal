#ifndef ARENASTATE_HPP
#define ARENASTATE_HPP
#include <iostream>
#include <string>
#include "GameState.hpp"
using namespace std;
class Character;
class MiniBoss;

class ArenaState : public GameState{
private:
    MiniBoss* enemy;
    string combatLog[3];
    int logCount;
    float heroVelocityY;
    bool heroBlocking;
    float quickCooldown;
    float powerCooldown;
    float specialCooldown;
    int enemyMode;
    int enemyAction;
    float enemyTimer;
    int enemyAttackCount;
    bool fightOver;
    bool heroWon;
    float endTimer;

    float getDistance() const;
    bool isHeroOnGround() const;
    int calculateDamage(const Character& attacker, const Character& defender, float multiplier) const;
    void heroAttack(int action);
    void updateHero(float dt);
    void updateEnemy(float dt);
    void enemyStrike();
    void keepApart();
    void checkFightEnd();
    void addLog(const string& line);
    void drawHealthBar(sf::RenderWindow& window, const Character& character, float x, float y) const;
    void drawCooldown(sf::RenderWindow& window, const string& label, float cooldown, float maxCooldown, float x) const;

public:
    static const int ENEMY_APPROACH = 0;
    static const int ENEMY_WINDUP = 1;
    static const int ENEMY_RECOVER = 2;
    static const float MIN_GAP;
    static const float LEFT_EDGE;
    static const float RIGHT_EDGE;
    static const float GRAVITY;
    static const float JUMP_SPEED;
    static const float QUICK_COOLDOWN;
    static const float POWER_COOLDOWN;
    static const float SPECIAL_COOLDOWN;

    ArenaState();
    ArenaState(Game* game, MiniBoss* enemy);
    ~ArenaState() override;
    ArenaState(const ArenaState& other) = delete;
    ArenaState& operator=(const ArenaState& other) = delete;

    void handleInput(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;
};

#endif