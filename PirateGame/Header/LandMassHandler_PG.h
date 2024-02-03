#pragma once

/// <summary>
/// This class is going to handle the land masses in the game.
/// It will be used to generate the land masses and store them in a vector.
/// </summary>

#include <SFML/Graphics.hpp>
#include <vector>

#include "Textures_PG.h"
#include "LandMass_PG.h"
#include "Hashmap_PG.h"
#include "Map.h"
#include "SoundManager_PG.h"

namespace PirateGame {
	class LandMassHandler {
	public:
		LandMassHandler(Map& map);
		~LandMassHandler();

		void addLandMasses(int numLandMasses, float minDistBetweenLandmasses);
		void drawLandMasses(sf::RenderWindow& window, Ship& ship);
	private:
		// Handle collisions between ship and land masses
		void handleCollisions(Ship& ship, sf::RenderWindow& window);
		int determineCollisionAxis(Ship& ship, LandMass* landMass);
		bool pixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2, unsigned alphaLimit = 0);

		// Vector to store the land masses
		std::vector<LandMass*> landMasses;

		// Hashmap to store the land masses
		Hashmap hashmap;

		// Map to store the land masses
		Map& map;

		// Temporary string for sound file
		std::string soundFile = "Sounds/bonk.wav";

		// SoundManager to play sounds
		SoundManager soundManager;

		// Temp function for bounds drawing
		void drawBounds(sf::RenderWindow& window, const sf::Sprite& rect);

		Textures texture;
	};
}


