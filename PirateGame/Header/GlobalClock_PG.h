#pragma once

/// Global clock to be used throughout the game

#include <SFML/Graphics.hpp>

namespace PirateGame {
	class GlobalClock {
	public:
		static GlobalClock& getInstance() {
			static GlobalClock instance;
			return instance;
		}

		void update() { clock.restart(); }

		sf::Time getElapsedTime() { return clock.getElapsedTime(); }
	private:
		// Private constructor
		GlobalClock() {};

		// Disable copy constructor and assignment operator
		GlobalClock(GlobalClock const&) = delete;
		void operator=(GlobalClock const&) = delete;

		sf::Clock clock;
	};
}