#include "BattleManager_PG.h"

using namespace PirateGame;

void BattleManager::startBattle(std::shared_ptr<ShipGroup> group1, ShipGroup* group2) { // Second group is a pointer because it is found through an iterator
	// Set the groups to be in combat
	group1->setInCombat(true);
	group2->setInCombat(true);
	group1->setIsInteracting(true);
	group2->setIsInteracting(true);

	// Add each group's ships to the other group's target ships vector
	for (auto& ship : group1->getEnemyShips()) {
		group2->addTarget(ship.get());
	}

	for (auto& ship : group2->getEnemyShips()) {
		group1->addTarget(ship.get());
	}

	// Add the groups to the battles vector
	battles.push_back(std::make_pair(group1, group2));
}

void BattleManager::updateBattles() const {
	// Loop through each battle
	for (auto& battle : battles) {
		// Grab the groups
		std::shared_ptr<ShipGroup> group1 = battle.first;
		ShipGroup* group2 = battle.second;

		// If either group is null, set the other group to not be in combat
		// Or, if either group has no ships, set the other group to not be in combat
		if (group1 == nullptr || group1->getEnemyShips().size() == 0
			|| group2 == nullptr || group2->getEnemyShips().size() == 0) {
			group2->setInCombat(false);
			group2->setIsInteracting(false);
			group1->setInCombat(false);
			group1->setIsInteracting(false);
		}
		else {
			// Reset both group's in combat and is interacting to true
			group1->setInCombat(true);
			group1->setIsInteracting(true);
			group2->setInCombat(true);
			group2->setIsInteracting(true);
		}
	}
}
