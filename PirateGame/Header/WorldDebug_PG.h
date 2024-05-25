#pragma once

/// Debug world

#include "World_PG.h"

namespace PirateGame {
	class DefaultDebugWorld : public World {
	public:
		DefaultDebugWorld(sf::RenderWindow* window) : World(window) {
			numLandMasses = 0;
			distanceBetweenLandMasses = 100.f;
			numEnemyShips = 5;
			distanceBetweenEnemyShips = 100.f;
		}
		~DefaultDebugWorld() {};

		void createWorld(sf::Event event) override;
	private:
		void updateGameLoop(sf::Event event) override;
	};
}