#pragma once

/// This class will handle the enemy ships in the game.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <random>

#include "GlobalContext_PG.h"
#include "QuadtreeHandler_PG.h"

#include "ShipGroup_PG.h"
#include "BattleManager_PG.h"

namespace PirateGame {
	class EnemyShipHandler {
	public:
		EnemyShipHandler(GlobalContext& context) : context(context) {}
		~EnemyShipHandler() {
			// Remove all enemy ships
			for (auto& ship : enemyShips) {
				QuadtreeHandler::enemyShipQuadtree->removeObject(ship.get());
			}
			enemyShips.clear();
		};

		void addEnemyShips(int numShips);
		void addEnemyShip(sf::Vector2f position, ShipClass type = ShipClass::Random);

		// Update and draw
		void update();
		void draw() const;

		// Setters
		void setPlayerShip(PlayerShip* playerShip) { this->playerShip = playerShip; }
		void setLandmasses(std::vector<std::shared_ptr<LandMass>> landmasses) { this->landmasses = landmasses; }
		void setInteractionChance(int interactionChance) { this->interactionChance = interactionChance; }

		// Getters
		std::vector<std::shared_ptr<EnemyShip>>& getEnemyShips() { return enemyShips; }
		std::vector<std::shared_ptr<ShipGroup>>& getShipGroups() { return shipGroups; }

	private:
		// Context
		GlobalContext& context;

		// helper functions
		void addEnemyShipsToChunk(Chunk& chunk, int numShips);
		void setShipGroupDestination(const std::shared_ptr<ShipGroup>& group) const;
		bool isDestinationReached(const std::shared_ptr<ShipGroup>& shipGroup) const;
		void updateGroupDestination(const std::shared_ptr<ShipGroup>& group) const;
		void updateGroupsNearPlayer();
		void updateShipsAsNotNearbyGroup(const std::shared_ptr<ShipGroup>& group) const;
		static void joinGroups(const std::shared_ptr<ShipGroup>& group1, ShipGroup* group2);
		void updateGroupCombat(const std::shared_ptr<ShipGroup>& group, std::set<EnemyShip*> nearbyShipsTotal);
		void interactWithNearbyShips(const std::shared_ptr<ShipGroup>& group, EnemyShip* otherShip);

		std::vector<std::shared_ptr<EnemyShip>> enemyShips;
		std::vector<std::shared_ptr<ShipGroup>> shipGroups;
		std::vector<std::shared_ptr<LandMass>> landmasses;
		PlayerShip* playerShip = nullptr;

		// Values
		const sf::Time enemyCannonCooldown = sf::seconds(2.f);
		const float maxDetectionDistance = 1000.f;
		const float audioRange = 5000.f;
		const float firingDistance = 1000.f;
		const float turningSpeed = 0.1f;
		const float enemySpeedMultiplier = 0.5f;
		const float minDistBetweenShips = 1000.f;
		const float interactionDistance = 1000.f;
		const float notNearbyAnymoreDistance = 3000.f;

		// Interaction chance is the 1/n chance of a specific interaction; For value x, chance of grouping, attacking, etc. is 1/x
		int interactionChance = 10;

		// DestinationReachedDistance is arbitrary; ship will choose a new destination at this distance from island. Does not account for diff sizes.
		const float destinationReachedDistance = 500.f;

		// BattleManager
		BattleManager battleManager;
	};
}