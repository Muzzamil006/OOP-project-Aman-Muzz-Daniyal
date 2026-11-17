#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "StoryManager.hpp"
#include "SaveManager.hpp"
#include "MinigameFactory.hpp"

using namespace std;

const unsigned int WINDOW_WIDTH = 960;
const unsigned int WINDOW_HEIGHT = 600;

class GameState;
class Hero;

class Game{
private:
    sf::RenderWindow window;
    sf::Font font;
    GameState* currentState;
    GameState* pendingState;
    Hero* hero;
    StoryManager storyManager;
    SaveManager saveManager;
    MinigameFactory minigameFactory;
    int chapter;

    static const int LAST_BUILT_CHAPTER = 2;

    void applyPendingState();
    void setHero(Hero* newHero);

public:
    Game();
    Game(unsigned int width, unsigned int height, const string& title);
    ~Game();
    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;

    void run();
    void requestState(GameState* state);
    void quit();

    const sf::Font& getFont() const;
    Hero* getHero() const;
    int getChapter() const;
    StoryManager& getStoryManager();
    const SaveManager& getSaveManager() const;

    void showMenu();
    void showHeroSelect();
    void startNewGame(int heroType);
    bool continueGame();
    void startChapter();
    void startMinigame();
    void startShop();
    void startArena();
    void showLevelUp();
    void finishChapter();
    void showGameOver(const string& message, bool canRetry);
};

#endif
