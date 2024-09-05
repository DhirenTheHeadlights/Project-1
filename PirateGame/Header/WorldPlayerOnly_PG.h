#pragma once

#include "World_PG.h"

namespace PirateGame {
	class PlayerOnlyWorld : public World {
	public:
		PlayerOnlyWorld(sf::RenderWindow* window) : World(window) {};
	private:
		void updateGameLoop(sf::Event event) override;
		void setUpEnemyShips() override;	
	};
}