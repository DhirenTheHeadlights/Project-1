#include "SoundManager_PG.h"

SoundManager::SoundManager(std::string pathToSound) {
	// Load the sound
	if (!collisionBuffer.loadFromFile(pathToSound)) {
		std::cout << "Error loading sound file." << std::endl;
	}

	// Set the sound to the buffer
	collisionSound.setBuffer(collisionBuffer);
}

void SoundManager::playSound() {
	// Only play if the sound is not already playing
	if (collisionSound.getStatus() != sf::Sound::Status::Playing) {
		collisionSound.play();
	}
}