#include <iostream>
#include "StoryState.hpp"
#include "Game.hpp"
#include "Hero.hpp"
#include "NPC.hpp"
#include "StoryManager.hpp"
#include "SoundManager.hpp"

using namespace std;

StoryState::StoryState() : GameState(), choiceButtons(), showingReply(false), replyText(""){
}

/*
Game loads the chapter into StoryManager first
Game creates StoryState and passes its own pointer
Initialize the GameState base class with that pointer
Start with no reply showing and an empty reply text
Build buttons from the current scene's stored choices
*/
StoryState::StoryState(Game* game) : GameState(game), choiceButtons(), showingReply(false), replyText(""){
    buildChoiceButtons();
}

/*
Called when this story screen is created and after changing scenes
Remove the previous scene's buttons
Ask StoryManager for the current scene
Read each choice's option text and create its button
Store the buttons in choiceButtons
Prepare buttons now; draw them only when choicesVisible() is true
*/
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

/*
Ask StoryManager whether the current dialogue line is the last line
Check whether the current scene contains choices
Return true only if both conditions hold and no reply is showing
Input handling and drawing use this result
*/
bool StoryState::choicesVisible() const{
    StoryManager& storyManager = game->getStoryManager();
    return !showingReply && storyManager.isLastLine() && storyManager.getCurrentScene().hasChoices();
}

/*
Ask StoryManager::nextScene() to advance the scene index
If another scene exists, its line index resets to zero
buildChoiceButtons() prepares the new scene's buttons
The game loop calls StoryState::draw() again
getCurrentScene() returns the newly selected scene
The new scene appears without creating another StoryState object
If no scene remains, ask Game to start the minigame
*/
void StoryState::goToNextScene(){
    if (game->getStoryManager().nextScene()){
        buildChoiceButtons();
    }
    else{
        game->startMinigame();
    }
}

/*
Called when the player presses Space or Enter, or clicks to continue
If a choice reply is showing, hide it and move to the next scene
If choices are visible, wait for a choice instead of advancing dialogue
Otherwise, ask StoryManager::nextLine() to advance
If no line remains, call goToNextScene()
The next draw uses the updated story position
*/
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

/*
Receive the index selected by a choice button or number key
Get that Choice from StoryManager's current scene
Copy its reply into replyText
Add any positive gold reward to the shared hero and mention it in the reply
Set showingReply to true and play the click sound
The next draw displays the reply instead of the dialogue line
The next advance moves to the next scene
*/
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

/*
The game loop passes each input event to this active screen
If choices are visible, check choice buttons and valid number keys
Call choose(index) for the selected option
Return without performing ordinary dialogue advancement
Otherwise, Space, Enter or a left mouse click calls advance()
*/
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

/*
The game loop calls update(dt) each frame while this state is active
Pass the elapsed time to the shared hero's update function
*/
void StoryState::update(float dt){
    game->getHero()->update(dt);
}

/*
The game loop calls draw() each frame while this state is active
Get the current scene from StoryManager and the shared hero from Game
Draw the scene background, grass, chapter title, scene title and gold
Draw the hero and the scene's NPC if one exists
Draw the dialogue panel
If showingReply is true, display the stored choice reply
Otherwise, get the current dialogue line and resolve HERO to the hero's name
Draw the speaker and text
Draw choice buttons when choicesVisible() is true
Otherwise, draw the instruction to advance
Changing the manager's indices makes the next frame display the new story data
*/
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
