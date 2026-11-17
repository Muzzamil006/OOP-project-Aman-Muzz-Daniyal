#include <iostream>
#include "SoundManager.hpp"
#include <fstream>

using namespace std;

SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager() : music(), clickBuffer(), clickSound(), clickLoaded(false), volume(50.0f){
    if (fileExists("assets/audio/click.wav")){
        clickLoaded = clickBuffer.loadFromFile("assets/audio/click.wav");
        clickSound.setBuffer(clickBuffer);
    }
}

SoundManager::SoundManager(float volume) : music(), clickBuffer(), clickSound(), clickLoaded(false), volume(volume){
    if (fileExists("assets/audio/click.wav")){
        clickLoaded = clickBuffer.loadFromFile("assets/audio/click.wav");
        clickSound.setBuffer(clickBuffer);
    }
}

SoundManager::~SoundManager(){
    music.stop();
    clickSound.stop();
}

SoundManager& SoundManager::getInstance(){
    if (instance == nullptr){
        instance = new SoundManager(50.0f);
    }
    return *instance;
}

void SoundManager::destroy(){
    delete instance;
    instance = nullptr;
}

bool SoundManager::fileExists(const string& path) const{
    ifstream file(path);
    return file.good();
}

void SoundManager::playMusic(const string& path){
    playMusic(path, volume);
}

void SoundManager::playMusic(const string& path, float musicVolume){
    if (!fileExists(path)){
        return;
    }
    if (music.openFromFile(path)){
        music.setLoop(true);
        music.setVolume(musicVolume);
        music.play();
    }
}

void SoundManager::stopMusic(){
    music.stop();
}

void SoundManager::playClick(){
    if (clickLoaded){
        clickSound.setVolume(volume);
        clickSound.play();
    }
}

void SoundManager::setVolume(float volume){
    this->volume = volume;
    music.setVolume(volume);
}

float SoundManager::getVolume() const{
    return volume;
}
