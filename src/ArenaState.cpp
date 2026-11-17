#include <iostream>
#include "ArenaState.hpp"
#include <cstdlib>
#include "Game.hpp"
#include "Hero.hpp"
#include "MiniBoss.hpp"
#include "CombatAction.hpp"
#include "SoundManager.hpp"

using namespace std;

const float ArenaState::MIN_GAP = 110.0f;
const float ArenaState::LEFT_EDGE = 20.0f;
const float ArenaState::RIGHT_EDGE = 880.0f;
const float ArenaState::GRAVITY = 1400.0f;
const float ArenaState::JUMP_SPEED = 620.0f;
const float ArenaState::QUICK_COOLDOWN = 0.5f;
const float ArenaState::POWER_COOLDOWN = 1.2f;
const float ArenaState::SPECIAL_COOLDOWN = 5.0f;

ArenaState::ArenaState()
    : GameState(), enemy(new MiniBoss()), combatLog(), heroVelocityY(0.0f), heroBlocking(false), quickCooldown(0.0f),
      powerCooldown(0.0f), specialCooldown(0.0f), enemyMode(ENEMY_APPROACH), enemyAction(ACTION_ADVANCE), enemyTimer(0.0f),
      enemyAttackCount(0), fightOver(false), heroWon(false), endTimer(0.0f){
}

ArenaState::ArenaState(Game* game, MiniBoss* enemy)
    : GameState(game), enemy(enemy), combatLog(), heroVelocityY(0.0f), heroBlocking(false), quickCooldown(0.0f),
      powerCooldown(0.0f), specialCooldown(0.0f), enemyMode(ENEMY_APPROACH), enemyAction(ACTION_ADVANCE), enemyTimer(0.0f),
      enemyAttackCount(0), fightOver(false), heroWon(false), endTimer(0.0f){
    game->getHero()->setHeightAboveGround(0.0f);
    addLog(enemy->getName() + " blocks the path. Fight!");
}

ArenaState::~ArenaState(){
    delete enemy;
}

float ArenaState::getDistance() const{
    return enemy->getCenterX() - game->getHero()->getCenterX();
}

bool ArenaState::isHeroOnGround() const{
    return game->getHero()->getHeightAboveGround() <= 0.0f;
}

int ArenaState::calculateDamage(const Character& attacker, const Character& defender, float multiplier) const{
    int base = attacker.getTotalStats().getStrength() + 2 + rand() % 4;
    int damage = (int)(base * multiplier) - defender.getTotalStats().getDefence() / 2;
    if (damage < 1){
        damage = 1;
    }
    return damage;
}

void ArenaState::keepApart(){
    Hero* hero = game->getHero();
    if (hero->getPositionX() < LEFT_EDGE){
        hero->setPositionX(LEFT_EDGE);
    }
    if (enemy->getPositionX() > RIGHT_EDGE){
        enemy->setPositionX(RIGHT_EDGE);
    }
    if (getDistance() < MIN_GAP){
        hero->setPositionX(enemy->getPositionX() - MIN_GAP);
    }
    if (hero->getPositionX() < LEFT_EDGE){
        hero->setPositionX(LEFT_EDGE);
        enemy->setPositionX(LEFT_EDGE + MIN_GAP);
    }
}

void ArenaState::heroAttack(int action){
    Hero* hero = game->getHero();
    float distance = getDistance();
    if (action == ACTION_QUICK_ATTACK && quickCooldown <= 0.0f){
        quickCooldown = QUICK_COOLDOWN;
        hero->getSprite().playAttack();
        if (distance <= Enemy::MELEE_RANGE){
            int damage = calculateDamage(*hero, *enemy, 1.0f);
            enemy->takeDamage(damage);
            addLog("Quick attack hits for " + to_string(damage) + ".");
        }
    }
    else if (action == ACTION_POWER_ATTACK && powerCooldown <= 0.0f){
        powerCooldown = POWER_COOLDOWN;
        hero->getSprite().playAttack();
        if (distance <= Enemy::MELEE_RANGE){
            int damage = calculateDamage(*hero, *enemy, 2.0f);
            enemy->takeDamage(damage);
            enemy->moveBy(40.0f);
            addLog("Power attack smashes for " + to_string(damage) + "!");
        }
    }
    else if (action == ACTION_SPECIAL && specialCooldown <= 0.0f){
        if (distance > hero->getSpecialRange()){
            addLog("Too far away for " + hero->getSpecialName() + ".");
            return;
        }
        specialCooldown = SPECIAL_COOLDOWN;
        hero->getSprite().playAttack();
        int damage = hero->useSpecial(*enemy);
        addLog(hero->getSpecialName() + " deals " + to_string(damage) + "!");
    }
    else{
        return;
    }
    SoundManager::getInstance().playClick();
    keepApart();
}

void ArenaState::updateHero(float dt){
    Hero* hero = game->getHero();
    quickCooldown -= dt;
    powerCooldown -= dt;
    specialCooldown -= dt;

    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    heroBlocking = down && isHeroOnGround();
    if (!heroBlocking){
        float speed = (180.0f + hero->getTotalStats().getAgility() * 8.0f) * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            hero->moveBy(-speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            hero->moveBy(speed);
        }
    }

    if (!isHeroOnGround() || heroVelocityY > 0.0f){
        float height = hero->getHeightAboveGround() + heroVelocityY * dt;
        heroVelocityY -= GRAVITY * dt;
        if (height <= 0.0f){
            height = 0.0f;
            heroVelocityY = 0.0f;
        }
        hero->setHeightAboveGround(height);
    }
    keepApart();
}

void ArenaState::updateEnemy(float dt){
    enemyTimer -= dt;
    if (enemyMode == ENEMY_APPROACH){
        if (getDistance() > Enemy::MELEE_RANGE - 20.0f){
            enemy->moveBy(-(90.0f + enemy->getTotalStats().getAgility() * 5.0f) * dt);
            keepApart();
        }
        else{
            enemyAttackCount += 1;
            enemyAction = enemy->chooseAction(getDistance(), enemyAttackCount);
            if (enemyAction == ACTION_REST){
                int amount = 6 + enemy->getTotalStats().getVitality() * 2;
                enemy->heal(amount);
                addLog(enemy->getName() + " recovers " + to_string(amount) + " HP.");
                enemyMode = ENEMY_RECOVER;
                enemyTimer = 1.0f;
            }
            else{
                enemyMode = ENEMY_WINDUP;
                enemyTimer = 0.45f;
                if (enemyAction == ACTION_POWER_ATTACK){
                    enemyTimer = 0.8f;
                }
                else if (enemyAction == ACTION_SPECIAL){
                    enemyTimer = 1.0f;
                    addLog(enemy->getName() + " prepares " + enemy->getSpecialName() + "... JUMP!");
                }
            }
        }
    }
    else if (enemyMode == ENEMY_WINDUP && enemyTimer <= 0.0f){
        enemyStrike();
        enemyMode = ENEMY_RECOVER;
        enemyTimer = 0.9f;
    }
    else if (enemyMode == ENEMY_RECOVER && enemyTimer <= 0.0f){
        enemyMode = ENEMY_APPROACH;
    }
}

void ArenaState::enemyStrike(){
    Hero* hero = game->getHero();
    enemy->getSprite().playAttack();
    int damage = 0;
    if (enemyAction == ACTION_SPECIAL){
        if (!isHeroOnGround()){
            addLog("You jumped over the " + enemy->getSpecialName() + "!");
            return;
        }
        if (getDistance() > 320.0f){
            addLog(enemy->getSpecialName() + " falls short.");
            return;
        }
        damage = enemy->getSpecialDamage() + enemy->getTotalStats().getStrength() - hero->getTotalStats().getDefence() / 2;
    }
    else{
        if (getDistance() > Enemy::MELEE_RANGE + 10.0f){
            addLog(enemy->getName() + " swings at the air.");
            return;
        }
        float multiplier = enemyAction == ACTION_POWER_ATTACK ? 1.8f : 1.0f;
        damage = calculateDamage(*enemy, *hero, multiplier);
    }
    if (damage < 1){
        damage = 1;
    }
    if (heroBlocking){
        damage = damage / 3 + 1;
        addLog("Blocked! You take only " + to_string(damage) + ".");
    }
    else{
        addLog(enemy->getName() + " hits you for " + to_string(damage) + ".");
        hero->moveBy(-35.0f);
    }
    hero->takeDamage(damage);
    keepApart();
}

void ArenaState::checkFightEnd(){
    if (fightOver){
        return;
    }
    Hero* hero = game->getHero();
    if (!enemy->isAlive()){
        fightOver = true;
        heroWon = true;
        endTimer = 3.0f;
        hero->addGold(enemy->getRewardGold());
        hero->addShard();
        addLog("Victory! +" + to_string(enemy->getRewardGold()) + " gold, +" + to_string(enemy->getRewardExperience()) + " XP, crystal shard!");
        if (hero->addExperience(enemy->getRewardExperience())){
            addLog(hero->getName() + " reached level " + to_string(hero->getLevel()) + "!");
        }
    }
    else if (!hero->isAlive()){
        fightOver = true;
        heroWon = false;
        endTimer = 2.0f;
        addLog(hero->getName() + " has fallen...");
    }
}

void ArenaState::addLog(const string& line){
    combatLog.push_back(line);
    if (combatLog.size() > 3){
        combatLog.erase(combatLog.begin());
    }
}

void ArenaState::handleInput(const sf::Event& event){
    if (fightOver || event.type != sf::Event::KeyPressed){
        return;
    }
    sf::Keyboard::Key key = event.key.code;
    if ((key == sf::Keyboard::W || key == sf::Keyboard::Up || key == sf::Keyboard::Space) && isHeroOnGround()){
        heroVelocityY = JUMP_SPEED;
        game->getHero()->setHeightAboveGround(0.1f);
    }
    if (heroBlocking){
        return;
    }
    if (key == sf::Keyboard::J || key == sf::Keyboard::Z){
        heroAttack(ACTION_QUICK_ATTACK);
    }
    else if (key == sf::Keyboard::K || key == sf::Keyboard::X){
        heroAttack(ACTION_POWER_ATTACK);
    }
    else if (key == sf::Keyboard::L || key == sf::Keyboard::C){
        heroAttack(ACTION_SPECIAL);
    }
}

void ArenaState::update(float dt){
    game->getHero()->update(dt);
    enemy->update(dt);
    if (fightOver){
        endTimer -= dt;
        if (endTimer <= 0.0f){
            if (heroWon && game->getHero()->getStatPoints() > 0){
                game->showLevelUp();
            }
            else if (heroWon){
                game->finishChapter();
            }
            else{
                game->showGameOver("Your hero has fallen in the arena.", true);
            }
        }
        return;
    }
    updateHero(dt);
    updateEnemy(dt);
    checkFightEnd();
}

void ArenaState::drawHealthBar(sf::RenderWindow& window, const Character& character, float x, float y) const{
    float width = 300.0f;
    float ratio = (float)character.getHealth() / (float)character.getMaxHealth();
    sf::RectangleShape back(sf::Vector2f(width, 20.0f));
    back.setPosition(x, y);
    back.setFillColor(sf::Color(60, 20, 20));
    back.setOutlineColor(sf::Color::Black);
    back.setOutlineThickness(2.0f);
    window.draw(back);
    sf::RectangleShape front(sf::Vector2f(width * ratio, 20.0f));
    front.setPosition(x, y);
    front.setFillColor(sf::Color(60, 190, 80));
    window.draw(front);
    drawText(window, character.getName() + " - " + character.getTitle(), 17, x, y - 26.0f, sf::Color(40, 25, 10));
    drawText(window, to_string(character.getHealth()) + " / " + to_string(character.getMaxHealth()), 14, x + 5.0f, y + 1.0f);
}

void ArenaState::drawCooldown(sf::RenderWindow& window, const string& label, float cooldown, float maxCooldown, float x) const{
    float ratio = 0.0f;
    if (cooldown > 0.0f){
        ratio = cooldown / maxCooldown;
    }
    sf::RectangleShape box(sf::Vector2f(200.0f, 34.0f));
    box.setPosition(x, 545.0f);
    box.setFillColor(ratio > 0.0f ? sf::Color(45, 45, 55) : sf::Color(60, 90, 60));
    box.setOutlineColor(sf::Color(230, 190, 70));
    box.setOutlineThickness(2.0f);
    window.draw(box);
    sf::RectangleShape fill(sf::Vector2f(200.0f * ratio, 34.0f));
    fill.setPosition(x, 545.0f);
    fill.setFillColor(sf::Color(20, 20, 25, 200));
    window.draw(fill);
    drawText(window, label, 16, x + 10.0f, 552.0f);
}

void ArenaState::draw(sf::RenderWindow& window) const{
    const Hero* hero = game->getHero();
    sf::RectangleShape sky(sf::Vector2f((float)WINDOW_WIDTH, 330.0f));
    sky.setFillColor(sf::Color(150, 195, 230));
    window.draw(sky);
    sf::RectangleShape wall(sf::Vector2f((float)WINDOW_WIDTH, 110.0f));
    wall.setPosition(0.0f, 240.0f);
    wall.setFillColor(sf::Color(190, 160, 120));
    window.draw(wall);
    sf::RectangleShape sand(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT - 350.0f));
    sand.setPosition(0.0f, 350.0f);
    sand.setFillColor(sf::Color(225, 190, 130));
    window.draw(sand);

    hero->draw(window, game->getFont());
    enemy->draw(window, game->getFont());

    if (!fightOver && enemyMode == ENEMY_WINDUP){
        float markY = Character::GROUND_Y - enemy->getSprite().getHeight() - 40.0f;
        drawText(window, enemyAction == ACTION_SPECIAL ? "!!" : "!", 34, enemy->getCenterX() - 8.0f, markY, sf::Color(220, 30, 30));
    }
    if (heroBlocking){
        drawText(window, "BLOCK", 16, hero->getCenterX() - 25.0f, Character::GROUND_Y - 230.0f, sf::Color(30, 60, 160));
    }

    drawHealthBar(window, *hero, 30.0f, 45.0f);
    drawHealthBar(window, *enemy, 630.0f, 45.0f);
    drawText(window, "Level " + to_string(hero->getLevel()) + "   Shards " + to_string(hero->getShards()) + "/4", 14, 30.0f, 72.0f, sf::Color(40, 25, 10));

    drawPanel(window, 15.0f, 440.0f, 930.0f, 150.0f);
    for (int i = 0; i < (int)combatLog.size(); i += 1){
        drawText(window, combatLog[i], 15, 28.0f, 447.0f + i * 19.0f, i == (int)combatLog.size() - 1 ? sf::Color::White : sf::Color(170, 170, 170));
    }
    drawText(window, "Move: A/D or Arrows    Jump: W / Up    Block: S / Down", 14, 28.0f, 512.0f, sf::Color(200, 200, 200));
    drawCooldown(window, "J / Z  Quick Attack", quickCooldown, QUICK_COOLDOWN, 28.0f);
    drawCooldown(window, "K / X  Power Attack", powerCooldown, POWER_COOLDOWN, 243.0f);
    drawCooldown(window, "L / C  " + hero->getSpecialName(), specialCooldown, SPECIAL_COOLDOWN, 458.0f);
}
