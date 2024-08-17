#pragma once

#include <SFML/Graphics.hpp>

namespace PirateGame {
	class GlobalClock {
	public:
		// Call this at the beginning of each frame
		void update() {
			dt = clock.restart();
		}

		// Get the same delta time for all classes
		sf::Time getDeltaTime() const {
			return dt;
		}

	private:
		sf::Clock clock;
		sf::Time dt;  // Store the delta time for each frame
	};
}
