#pragma once

/// This class will handle the battle system in the game.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "ShipGroup_PG.h"

namespace PirateGame {
	class BattleManager {
	public:
		BattleManager() {};
		~BattleManager() {};

		void startBattle(std::shared_ptr<ShipGroup> group1, ShipGroup* group2);
		void updateBattles();
	private:
		// Store battles
		std::vector<std::pair<std::shared_ptr<ShipGroup>, ShipGroup*>> battles;
	};
}