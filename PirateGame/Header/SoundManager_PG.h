#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <map>

/// <summary>
/// This class is used to manage the sounds in the game.
/// </summary>

namespace PirateGame {
	class SoundManager {
	public:
		SoundManager(int amountOfSoundsToReserve = 10) { sounds.reserve(amountOfSoundsToReserve); }

		// Play the sound and set the buffer
		void playSound(const std::string& soundName);
		void loadSoundBuffer(const std::string& soundName, const std::string& filePath);

	private:
		// Find an available sound
		sf::Sound& findAvailableSound();

		// Buffer and sound for collision
		sf::SoundBuffer collisionBuffer;
		sf::Sound sound;
		
		// Vector of sounds
		std::vector<sf::Sound> sounds;
		std::map<std::string, sf::SoundBuffer> soundBuffers;
	};
}

