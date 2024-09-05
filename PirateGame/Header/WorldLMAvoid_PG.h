#pragma once

/// A unit test to see if the pathfinding algorithm works

#include "World_PG.h"

namespace PirateGame {
	class LMAvoidWorld : public World {
	public:
		LMAvoidWorld(sf::RenderWindow* window) : World(window) {};
	private:
		void updateGameLoop(sf::Event event) override;
		void setUpLandMasses() override;
		void setUpEnemyShips() override;
	};
}