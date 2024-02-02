#pragma once
#include <SFML/Audio.hpp>
#include <iostream>

/// <summary>
/// This class is used to manage the sounds in the game.
/// </summary>

namespace PirateGame {
	class SoundManager {
	public:
		SoundManager(std::string pathToSound);

		// Play the sound on command
		void playSound();

	private:
		// Buffer and sound for collision
		sf::SoundBuffer collisionBuffer;
		sf::Sound collisionSound;
	};
}

