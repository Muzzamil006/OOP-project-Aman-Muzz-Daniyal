#include <iostream>
#include "Game.hpp"
#include "GameState.hpp"
#include "Hero.hpp"
#include "MenuState.hpp"
#include "HeroSelectState.hpp"
#include "StoryState.hpp"
#include "MinigameState.hpp"
#include "ShopState.hpp"
#include "SoundManager.hpp"

using namespace std;

Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Teen Talwar", sf::Style::Titlebar | sf::Style::Close),
      font(), currentState(nullptr), pendingState(nullptr), hero(nullptr), storyManager(), saveManager(), minigameFactory(), chapter(1){
    window.setFramerateLimit(60);
    font.loadFromFile("assets/fonts/DejaVuSans.ttf");
    SoundManager::getInstance().playMusic("assets/audio/theme.ogg");
    currentState = new MenuState(this);
}

Game::Game(unsigned int width, unsigned int height, const string& title)
    : window(sf::VideoMode(width, height), title, sf::Style::Titlebar | sf::Style::Close),
      font(), currentState(nullptr), pendingState(nullptr), hero(nullptr), storyManager(), saveManager("save.txt"), minigameFactory(4), chapter(1){
    window.setFramerateLimit(60);
    font.loadFromFile("assets/fonts/DejaVuSans.ttf");
    SoundManager::getInstance().playMusic("assets/audio/theme.ogg");
    currentState = new MenuState(this);
}

Game::~Game(){
    delete currentState;
    delete pendingState;
    delete hero;
    SoundManager::destroy();
}

void Game::run(){
    sf::Clock clock;
    while (window.isOpen()){
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            else if (currentState != nullptr){
                currentState->handleInput(event);
            }
        }
        if (currentState != nullptr){
            currentState->update(dt);
        }
        applyPendingState();
        window.clear(sf::Color(20, 20, 35));
        if (currentState != nullptr){
            currentState->draw(window);
        }
        window.display();
    }
}

void Game::requestState(GameState* state){
    delete pendingState;
    pendingState = state;
}

void Game::applyPendingState(){
    if (pendingState != nullptr){
        delete currentState;
        currentState = pendingState;
        pendingState = nullptr;
    }
}

void Game::quit(){
    window.close();
}

const sf::Font& Game::getFont() const{
    return font;
}

Hero* Game::getHero() const{
    return hero;
}

int Game::getChapter() const{
    return chapter;
}

StoryManager& Game::getStoryManager(){
    return storyManager;
}

const SaveManager& Game::getSaveManager() const{
    return saveManager;
}

void Game::setHero(Hero* newHero){
    delete hero;
    hero = newHero;
}

void Game::showMenu(){
    requestState(new MenuState(this));
}

void Game::showHeroSelect(){
    requestState(new HeroSelectState(this));
}

void Game::startNewGame(int heroType){
    setHero(Hero::create(heroType));
    chapter = 1;
    startChapter();
}

bool Game::continueGame(){
    int savedChapter = 1;
    Hero* loadedHero = saveManager.load(savedChapter);
    if (loadedHero == nullptr){
        return false;
    }
    setHero(loadedHero);
    chapter = savedChapter;
    startChapter();
    return true;
}

void Game::startChapter(){
    if (chapter > LAST_BUILT_CHAPTER){
        showMenu();
        return;
    }
    hero->setPositionX(150.0f);
    if (!storyManager.loadChapter(chapter)){
        showMenu();
        return;
    }
    requestState(new StoryState(this));
}

void Game::startMinigame(){
    requestState(new MinigameState(this, minigameFactory.create(chapter)));
}

void Game::startShop(){
    requestState(new ShopState(this));
}


void Game::finishChapter(){
    chapter += 1;
    saveManager.save(*hero, chapter);
    startChapter();
}
