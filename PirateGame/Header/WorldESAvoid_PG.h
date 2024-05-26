#pragma once

/// Unit test for enemy ship collision avoidance

#include "World_PG.h"

namespace PirateGame {
	class ESAvoidWorld : public World {
	public:
		ESAvoidWorld(sf::RenderWindow* window) : World(window) {};
		~ESAvoidWorld() {};

		void createWorld(sf::Event event) override;
	private:
		void updateGameLoop(sf::Event event) override;
		void setUpLandMasses() override;
		void setUpEnemyShips() override;
	};
}