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
        CannonShot
        // Add more identifiers as needed
    };

    class GlobalSoundManager {
    public:
        // Deletes copy constructor and assignment operator to ensure singleton uniqueness
        GlobalSoundManager(const GlobalSoundManager&) = delete;
        GlobalSoundManager& operator=(const GlobalSoundManager&) = delete;

        // Provides global access to the single instance of SoundManager
        static GlobalSoundManager& getInstance() {
            static GlobalSoundManager instance;
            return instance;
        }

        // Loads and plays sounds by their identifiers
        void playSound(SoundId soundId);

    private:
        // Private constructor for singleton pattern
        GlobalSoundManager() {
            // Set up the sound paths
            setSoundFilePath(SoundId::Bonk, "Sounds/bonk.wav");
            setSoundFilePath(SoundId::Select, "Sounds/select.wav");
            setSoundFilePath(SoundId::CannonShot, "Sounds/cannon_shot.wav");

            // Load the sound buffers
            for (auto& sound : soundFilePaths) {
				loadSound(sound.first);
			}
        }

        // Helper methods
        sf::Sound& findAvailableSound();
        void loadSound(SoundId soundId);
        void setSoundFilePath(SoundId soundId, const std::string& filePath);

        // Sound data containers
        std::map<SoundId, std::string> soundFilePaths;
        std::map<SoundId, sf::SoundBuffer> soundBuffers;
        std::vector<sf::Sound> sounds;
    };

}
