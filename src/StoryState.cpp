#include <iostream>
#include "StoryState.hpp"
#include "Game.hpp"
#include "Hero.hpp"
#include "NPC.hpp"
#include "StoryManager.hpp"
#include "SoundManager.hpp"

using namespace std;
// goToNextScene()
//     
// nextScene() changes sceneIndex from 0 to 1
//     
// buildChoiceButtons() prepares scene 1's buttons
//     
// Game loop calls StoryState::draw()
//     
// getCurrentScene() returns scene 1
//     
// Scene 1 appears on screen
StoryState::StoryState() : GameState(), choiceButtons(), showingReply(false), replyText(""){
}

StoryState::StoryState(Game* game) : GameState(game), choiceButtons(), showingReply(false), replyText(""){
    buildChoiceButtons();
}

void StoryState::buildChoiceButtons(){
    choiceButtons.clear();
    const StoryScene& scene = game->getStoryManager().getCurrentScene();
    for (int i = 0; i < scene.getChoiceCount(); i += 1){
        Button button(to_string(i + 1) + ". " + scene.getChoice(i).getText(), game->getFont(), 70.0f + i * 420.0f, 520.0f, 400.0f, 40.0f);
        if (i == 0){
            button.setColor(sf::Color(190, 140, 40));
        }
        choiceButtons.push_back(button);
    }
}

bool StoryState::choicesVisible() const{
    StoryManager& storyManager = game->getStoryManager();
    return !showingReply && storyManager.isLastLine() && storyManager.getCurrentScene().hasChoices();
}

void StoryState::goToNextScene(){
    if (game->getStoryManager().nextScene()){
        buildChoiceButtons();
    }
    else{
        game->startMinigame();
    }
}

void StoryState::advance(){
    if (showingReply){
        showingReply = false;
        goToNextScene();
        return;
    }
    if (choicesVisible()){
        return;
    }
    if (!game->getStoryManager().nextLine()){
        goToNextScene();
    }
}

void StoryState::choose(int index){
    const Choice& choice = game->getStoryManager().getCurrentScene().getChoice(index);
    replyText = choice.getReply();
    if (choice.getRewardGold() > 0){
        game->getHero()->addGold(choice.getRewardGold());
        replyText += "  (+" + to_string(choice.getRewardGold()) + " gold)";
    }
    showingReply = true;
    SoundManager::getInstance().playClick();
}

void StoryState::handleInput(const sf::Event& event){
    if (choicesVisible()){
        for (int i = 0; i < (int)choiceButtons.size(); i += 1){
            if (choiceButtons[i].isClicked(event)){
                choose(i);
                return;
            }
        }
        if (event.type == sf::Event::KeyPressed){
            int index = event.key.code - sf::Keyboard::Num1;
            if (index >= 0 && index < (int)choiceButtons.size()){
                choose(index);
            }
        }
        return;
    }
    if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter)){
        advance();
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
        advance();
    }
}

void StoryState::update(float dt){
    game->getHero()->update(dt);
}

void StoryState::draw(sf::RenderWindow& window) const{
    StoryManager& storyManager = game->getStoryManager();
    const StoryScene& scene = storyManager.getCurrentScene();
    const Hero* hero = game->getHero();

    sf::RectangleShape background(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    background.setFillColor(scene.getBackground());
    window.draw(background);
    sf::RectangleShape grass(sf::Vector2f((float)WINDOW_WIDTH, 200.0f));
    grass.setPosition(0.0f, Character::GROUND_Y - 60.0f);
    grass.setFillColor(sf::Color(scene.getBackground().r / 2, scene.getBackground().g / 2 + 30, scene.getBackground().b / 2));
    window.draw(grass);

    drawText(window, storyManager.getChapterTitle(), 22, 20.0f, 15.0f, sf::Color(240, 200, 80));
    drawText(window, scene.getTitle(), 16, 20.0f, 45.0f, sf::Color(220, 220, 220));
    drawText(window, "Gold: " + to_string(hero->getGold()), 16, 820.0f, 15.0f, sf::Color(240, 200, 80));

    hero->draw(window, game->getFont());
    if (scene.getNpc() != nullptr){
        scene.getNpc()->draw(window, game->getFont());
    }

    drawPanel(window, 40.0f, 435.0f, 880.0f, 150.0f);
    string speaker = "";
    string text = "";
    if (showingReply){
        if (scene.getNpc() != nullptr){
            speaker = scene.getNpc()->getName();
        }
        text = replyText;
    }
    else{
        const DialogueLine& line = storyManager.getCurrentLine();
        speaker = line.isHeroLine() ? hero->getName() : line.getSpeaker();
        text = line.getText();
    }
    drawText(window, speaker + ":", 18, 60.0f, 447.0f, sf::Color(240, 200, 80));
    drawText(window, text, 17, 60.0f, 477.0f);
    if (choicesVisible()){
        for (int i = 0; i < (int)choiceButtons.size(); i += 1){
            choiceButtons[i].draw(window);
        }
    }
    else{
        drawText(window, "Space / Enter / Click", 13, 770.0f, 560.0f, sf::Color(160, 160, 160));
    }
}
