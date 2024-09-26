#pragma once

#include <SFML/Graphics.hpp>

namespace PirateGame {
	namespace Clock {
		inline sf::Clock clock;
		inline sf::Time dt;

		inline sf::Clock autosaveClock;

		// Call this at the beginning of each frame
		inline void update() {
			dt = clock.restart();
		}

		// Get the same delta time for all classes
		inline sf::Time getDeltaTime() {
			return dt;
		}

		inline bool getAutosaveTrigger() {
			if (autosaveClock.getElapsedTime().asSeconds() > 15) {
				autosaveClock.restart();
				return true;
			}
			return false;
		}
	};
}
