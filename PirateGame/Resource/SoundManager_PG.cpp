#include "SoundManager_PG.h"

using namespace PirateGame;

SoundManager::SoundManager(std::string pathToSound) {
	// Load the sound
	if (!collisionBuffer.loadFromFile(pathToSound)) {
		std::cout << "Error loading sound file." << std::endl;
	}

	// Set the sound to the buffer
	sound.setBuffer(collisionBuffer);
}

void SoundManager::playSound() {
	// Only play if the sound is not already playing
	if (sound.getStatus() != sf::Sound::Status::Playing) {
		sound.play();
	}
}