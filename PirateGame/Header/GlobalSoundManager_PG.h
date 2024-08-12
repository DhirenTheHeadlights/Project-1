#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <iostream>

namespace PirateGame {

    // Define the enum class for sound identifiers
    enum class SoundId {
        Bonk,
        Select,
        CannonShot,
        CannonImpact,
        // Add more identifiers as needed
    };

    class GlobalSoundManager {
    public:
        GlobalSoundManager() {
            // Set up the sound paths
            setSoundFilePath(SoundId::Bonk, "Sounds/bonk.wav");
            setSoundFilePath(SoundId::Select, "Sounds/select.wav");
            setSoundFilePath(SoundId::CannonShot, "Sounds/cannon_shot.wav");
            setSoundFilePath(SoundId::CannonImpact, "Sounds/cannon_impact.wav");

            // Load the sound buffers
            for (auto& sound : soundFilePaths) {
                loadSound(sound.first);
            }
        }

        // Loads and plays sounds by their identifiers
        void playSound(SoundId soundId);

        // Setters
        void setSoundVolume(float volume) { soundVolume = volume; }

    private:
        // Helper methods
        sf::Sound& findAvailableSound();
        void loadSound(SoundId soundId);
        void setSoundFilePath(SoundId soundId, const std::string& filePath);

        // Sound data containers
        std::map<SoundId, std::string> soundFilePaths;
        std::map<SoundId, sf::SoundBuffer> soundBuffers;
        std::vector<sf::Sound> sounds;

        // Values
        float soundVolume = 50;
    };

}
