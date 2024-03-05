#include "SoundManager_PG.h"

using namespace PirateGame;

void SoundManager::loadSoundBuffer(const std::string& soundName, const std::string& filePath) {
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(filePath)) {
        soundBuffers[soundName] = buffer;
    }
    else {
        std::cerr << "Failed to load sound buffer: " << filePath << std::endl;
    }
}

void SoundManager::playSound(const std::string& soundName) {
    if (soundBuffers.find(soundName) != soundBuffers.end()) {
        sf::Sound& sound = findAvailableSound();
        sound.setBuffer(soundBuffers[soundName]);
        sound.setVolume(GlobalValues::getInstance().getGlobalVolume());
        sound.play();
    }
    else {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}

sf::Sound& SoundManager::findAvailableSound() {
    for (auto& sound : sounds) {
        if (sound.getStatus() != sf::Sound::Playing) {
            return sound;
        }
    }

    // All sounds are playing, so we add a new one
    sounds.emplace_back();
    return sounds.back();
}
