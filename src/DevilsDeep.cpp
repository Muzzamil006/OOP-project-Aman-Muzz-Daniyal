#include <iostream>
#include "DevilsDeep.hpp"
#include <cstdlib>
#include "CharacterSprite.hpp"

using namespace std;

const float DevilsDeep::LEFT = 40.0f;
const float DevilsDeep::TOP = 90.0f;
const float DevilsDeep::RIGHT = 920.0f;
const float DevilsDeep::BOTTOM = 560.0f;

/*
Create a default DevilsDeep object
Initialize the Minigame base with a 30-second time limit
Use a target of 8 coins and 5 enemies
Call reset() to prepare the round
*/
DevilsDeep::DevilsDeep()
    : Minigame("Devil's Deep", "Collect coins with WASD / arrows. Avoid the Evil Aarijs!", 30.0f),
      playerPosition(), coins(), aarijPositions(), aarijVelocities(), coinsNeeded(8), aarijCount(5){
    reset();
}

/*
MinigameFactory creates DevilsDeep with chapter-specific settings
Initialize the Minigame base with the name, instructions and time limit
Store the required coin count and enemy count
Call reset() to prepare player, coins and enemies
*/
DevilsDeep::DevilsDeep(float timeLimit, int coinsNeeded, int aarijCount)
    : Minigame("Devil's Deep", "Collect " + to_string(coinsNeeded) + " coins with WASD / arrows. Avoid the Evil Aarijs!", timeLimit),
      playerPosition(), coins(), aarijPositions(), aarijVelocities(), coinsNeeded(coinsNeeded), aarijCount(aarijCount){
    reset();
}

/*
reset() or coin collection needs a random position
Generate x using the supplied minimum horizontal position
Generate y using the arena top and bottom bounds
Return the position as an sf::Vector2f
*/
sf::Vector2f DevilsDeep::randomPoint(float minX) const{
    float x = minX + rand() % (int)(RIGHT - minX - 40.0f);
    float y = TOP + rand() % (int)(BOTTOM - TOP - 40.0f);
    return sf::Vector2f(x, y);
}

/*
Called by construction, Start or Retry
Restore the timer and clear score, finished and won
Place the player at the starting position
Clear old coin positions, enemy positions and enemy velocities
Generate 12 coins at random positions
Generate the configured enemies with random positions and velocities
The round is ready to play
*/
void DevilsDeep::reset(){
    timeLeft = timeLimit;
    score = 0;
    finished = false;
    won = false;
    playerPosition = sf::Vector2f(80.0f, 300.0f);
    coins.clear();
    aarijPositions.clear();
    aarijVelocities.clear();
    for (int i = 0; i < 12; i += 1){
        coins.push_back(randomPoint(LEFT + 20.0f));
    }
    for (int i = 0; i < aarijCount; i += 1){
        aarijPositions.push_back(randomPoint(400.0f));
        float speedX = (float)(70 + rand() % 80);
        float speedY = (float)(70 + rand() % 80);
        if (rand() % 2 == 0){
            speedX = -speedX;
        }
        if (rand() % 2 == 0){
            speedY = -speedY;
        }
        aarijVelocities.push_back(sf::Vector2f(speedX, speedY));
    }
}

/*
MinigameState calls minigame->update(dt) while playing
Return immediately if the round has already finished
Reduce timeLeft using the inherited tickTimer(dt)
Read held WASD or arrow keys and move the player using elapsed time
Clamp the player position inside the arena
For each collected coin, increase score and relocate that coin
Move enemies and reverse velocity at arena boundaries
On enemy collision, call finish(score >= coinsNeeded) and return
If time runs out, call finish(score >= coinsNeeded)
Reaching the target alone does not end the round; collision or timeout does
MinigameState checks finished and switches to its result phase
*/
void DevilsDeep::update(float dt){
    if (finished){
        return;
    }
    tickTimer(dt);
    float speed = 260.0f * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        playerPosition.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        playerPosition.x += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        playerPosition.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        playerPosition.y += speed;
    }
    if (playerPosition.x < LEFT){
        playerPosition.x = LEFT;
    }
    if (playerPosition.x > RIGHT - 30.0f){
        playerPosition.x = RIGHT - 30.0f;
    }
    if (playerPosition.y < TOP){
        playerPosition.y = TOP;
    }
    if (playerPosition.y > BOTTOM - 30.0f){
        playerPosition.y = BOTTOM - 30.0f;
    }

    sf::FloatRect playerBox(playerPosition.x, playerPosition.y, 30.0f, 30.0f);
    for (int i = 0; i < (int)coins.size(); i += 1){
        sf::FloatRect coinBox(coins[i].x - 8.0f, coins[i].y - 8.0f, 16.0f, 16.0f);
        if (playerBox.intersects(coinBox)){
            score += 1;
            coins[i] = randomPoint(LEFT + 20.0f);
        }
    }

    for (int i = 0; i < (int)aarijPositions.size(); i += 1){
        aarijPositions[i] += aarijVelocities[i] * dt;
        if (aarijPositions[i].x < LEFT || aarijPositions[i].x > RIGHT - 36.0f){
            aarijVelocities[i].x = -aarijVelocities[i].x;
        }
        if (aarijPositions[i].y < TOP || aarijPositions[i].y > BOTTOM - 36.0f){
            aarijVelocities[i].y = -aarijVelocities[i].y;
        }
        sf::FloatRect aarijBox(aarijPositions[i].x, aarijPositions[i].y, 36.0f, 36.0f);
        if (playerBox.intersects(aarijBox)){
            finish(score >= coinsNeeded);
            return;
        }
    }

    if (timeLeft <= 0.0f){
        finish(score >= coinsNeeded);
    }
}

/*
MinigameState calls minigame->draw(window, font)
Draw the arena and all coin positions
Draw enemies and their labels
Draw the borrowed hero sprite, or a fallback rectangle if none is assigned
Display the collected coins, target and remaining time
MinigameState may then draw its result panel over this gameplay area
*/
void DevilsDeep::draw(sf::RenderWindow& window, const sf::Font& font) const{
    sf::RectangleShape arena(sf::Vector2f(RIGHT - LEFT, BOTTOM - TOP));
    arena.setPosition(LEFT, TOP);
    arena.setFillColor(sf::Color(90, 20, 15));
    arena.setOutlineColor(sf::Color(200, 80, 40));
    arena.setOutlineThickness(3.0f);
    window.draw(arena);

    sf::CircleShape coin(8.0f);
    coin.setFillColor(sf::Color(240, 200, 60));
    for (int i = 0; i < (int)coins.size(); i += 1){
        coin.setPosition(coins[i].x - 8.0f, coins[i].y - 8.0f);
        window.draw(coin);
    }

    sf::RectangleShape aarij(sf::Vector2f(36.0f, 36.0f));
    aarij.setFillColor(sf::Color(200, 30, 30));
    aarij.setOutlineColor(sf::Color::Black);
    aarij.setOutlineThickness(2.0f);
    sf::Text aarijLabel("Evil Aarij", font, 11);
    aarijLabel.setFillColor(sf::Color::White);
    for (int i = 0; i < (int)aarijPositions.size(); i += 1){
        aarij.setPosition(aarijPositions[i]);
        window.draw(aarij);
        aarijLabel.setPosition(aarijPositions[i].x - 8.0f, aarijPositions[i].y - 15.0f);
        window.draw(aarijLabel);
    }

    if (playerSprite != nullptr){
        playerSprite->draw(window, playerPosition.x + 15.0f, playerPosition.y + 30.0f, 0.75f / playerSprite->getScale());
    }
    else{
        sf::RectangleShape player(sf::Vector2f(30.0f, 30.0f));
        player.setPosition(playerPosition);
        player.setFillColor(sf::Color(80, 140, 240));
        window.draw(player);
    }

    sf::Text hud("Coins: " + to_string(score) + " / " + to_string(coinsNeeded) + "      Time: " + to_string((int)timeLeft) + "s", font, 20);
    hud.setFillColor(sf::Color(240, 200, 80));
    hud.setPosition(LEFT, 55.0f);
    window.draw(hud);
}
