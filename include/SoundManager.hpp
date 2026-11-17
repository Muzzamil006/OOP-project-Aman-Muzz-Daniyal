#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <iostream>
#include <string>
#include <SFML/Audio.hpp>

using namespace std;

class SoundManager{
private:
    sf::Music music;
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;
    bool clickLoaded;
    float volume;

    static SoundManager* instance;

    SoundManager();
    SoundManager(float volume);
    bool fileExists(const string& path) const;

public:
    ~SoundManager();
    SoundManager(const SoundManager& other) = delete;
    SoundManager& operator=(const SoundManager& other) = delete;

    static SoundManager& getInstance();
    static void destroy();

    void playMusic(const string& path);
    void playMusic(const string& path, float musicVolume);
    void stopMusic();
    void playClick();
    void setVolume(float volume);
    float getVolume() const;
};

#endif
