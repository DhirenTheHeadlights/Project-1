#pragma once

/// Global clock to be used throughout the game

#include <SFML/Graphics.hpp>

namespace PirateGame {
	class GlobalClock {
	public:
		void update() { clock.restart(); }

		sf::Time getElapsedTime() { return clock.getElapsedTime(); }
	private:
		sf::Clock clock;
	};
}