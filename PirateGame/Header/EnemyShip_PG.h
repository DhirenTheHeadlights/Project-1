#pragma once

/// This class is almost identical to PlayerShip, but it is used for the enemy ships.

#include <SFML/Graphics.hpp>

#include "EnemyShipInputHandler_PG.h"
#include "EnemyShipMovementHandler_PG.h"
#include "GlobalContext_PG.h"
#include "Ship_PG.h"

namespace PirateGame {
	class EnemyShip : public Ship {
	public:
		EnemyShip(GlobalContext& context) : Ship(context) {}

		// Custom functions
		void customShipSetUp() override;
		void customShipUpdate() override;
		void customShipDraw() override;

		// Setters
		void setTargetPosition(sf::Vector2f targetPos) {
			// Perform a dynamic cast to check if SIH and SMH are enemy types
			if (const auto SIHEnemy = dynamic_cast<EnemyShipInputHandler*>(SIH.get())) {
				SIHEnemy->setTargetPos(targetPos);
			}
			if (const auto SMHEnemy = dynamic_cast<EnemyShipMovementHandler*>(SMH.get())) {
				SMHEnemy->setTargetPosition(targetPos);
			}
			this->targetPosition = targetPos;
		}

		void setDifficultyScaling(float difficultyScaling) {
			getSpecificShipProperties().baseSpeed *= difficultyScaling;
			getSpecificShipProperties().maxHealth *= difficultyScaling;
			getSpecificShipProperties().regenRate *= difficultyScaling;
			setHealth(this->getSpecificShipProperties().maxHealth);
		}

		// Overridden getters
		EnemyShipInputHandler* getInputHandler() override { return dynamic_cast<EnemyShipInputHandler*>(SIH.get()); };
		EnemyShipMovementHandler* getMovementHandler() override { return dynamic_cast<EnemyShipMovementHandler*>(SMH.get()); };
	private:
		// Variables
		sf::Vector2f targetPosition;
		bool debug = false;
	};
}

