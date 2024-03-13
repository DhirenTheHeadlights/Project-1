#include "GlobalSoundManager_PG.h"

using namespace PirateGame;

void GlobalSoundManager::setSoundFilePath(SoundId soundId, const std::string& filePath) {
    soundFilePaths[soundId] = filePath;
}

void GlobalSoundManager::loadSound(SoundId soundId) {
    auto filePathIt = soundFilePaths.find(soundId);
    if (filePathIt != soundFilePaths.end()) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(filePathIt->second)) {
            soundBuffers[soundId] = buffer;
        }
        else {
            std::cerr << "Failed to load sound buffer: " << filePathIt->second << std::endl;
        }
    }
    else {
        std::cerr << "Sound ID not found in file path map." << std::endl;
    }
}

void GlobalSoundManager::playSound(SoundId soundId) {
    auto bufferIt = soundBuffers.find(soundId);
    if (bufferIt != soundBuffers.end()) {
        sf::Sound& sound = findAvailableSound();
        sound.setBuffer(bufferIt->second);
        sound.play();
    }
    else {
        std::cerr << "Sound ID not found in loaded buffers. Did you forget to load it in the Global class?" << std::endl;
    }
}

sf::Sound& GlobalSoundManager::findAvailableSound() {
    for (auto& sound : sounds) {
        if (sound.getStatus() != sf::Sound::Playing) {
            return sound;
        }
    }

    sounds.emplace_back();
    return sounds.back();
}
