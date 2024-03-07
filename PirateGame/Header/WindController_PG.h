#pragma once

/// Simple wind controller class. It will control the wind direction and speed.

#include <SFML/Graphics.hpp>

namespace PirateGame {
	class WindController {
	public:
		WindController() {};
		~WindController() {};

		void update() {
			// For now, randomly change the wind direction and speed every windChangeTime seconds
			if (windChangeTimer.getElapsedTime() > windChangeTime) {
				windDirection = sf::Vector2f((float)(rand() % 3 - 1), (float)(rand() % 3 - 1));
				windSpeed = (float)(rand() % 10 + 1);
				windChangeTimer.restart();
			}
		}

		// Setters
		void setWindDirection(sf::Vector2f windDirection) { this->windDirection = windDirection; }
		void setWindSpeed(float windSpeed) { this->windSpeed = windSpeed; }

		// Getters
		sf::Vector2f getWindDirection() { return windDirection; }
		float getWindSpeed() { return windSpeed; }
	private:
		sf::Vector2f windDirection = sf::Vector2f(0.f, 0.f);
		float windSpeed = 0.f;

		sf::Clock windChangeTimer;
		sf::Time windChangeTime = sf::seconds(10.f);
	};
}