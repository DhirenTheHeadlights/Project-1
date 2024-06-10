#pragma once

/// Default world

#include "World_PG.h"

namespace PirateGame {
	class DefaultWorld : public World {
	public:
		DefaultWorld(sf::RenderWindow* window) : World(window) {
			numLandMasses = 5;
			distanceBetweenLandMasses = 1000.f;
			numEnemyShips = 5;
			distanceBetweenEnemyShips = 1000.f;
		}
		~DefaultWorld() {};

		void createWorld(sf::Event event) override;
	private:
		void updateGameLoop(sf::Event event) override;
	};
}