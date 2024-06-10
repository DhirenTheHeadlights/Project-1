#pragma once

/// Derived enemy movement handler class

#include "ShipMovementHandler_PG.h"
#include "AstarAlgorithm_PG.h"

namespace PirateGame {
	class EnemyShipMovementHandler : public ShipMovementHandler {
	public:
		EnemyShipMovementHandler(sf::Sprite& sprite) : ShipMovementHandler(sprite) {};
		~EnemyShipMovementHandler() {};

		// Movement function
		void update(float baseSpeed, sf::Vector2f sailRotation) override;

		// Setters
		void setTargetPosition(sf::Vector2f targetPos) { this->targetPos = targetPos; }
		void setEnemySpeedMultiplier(float enemySpeedMultiplier) { this->enemySpeedMultiplier = enemySpeedMultiplier; }
		void setDestination(sf::Vector2f destination) { 
			this->destination = destination;
			getAStar().setStartAndEndPoints(getSprite().getPosition(), destination);
		}
		void setActiveTowardsTarget(bool aimTowardsTarget) { this->activeTowardsTarget = aimTowardsTarget; }

		// Getters
		sf::Vector2f getDestination() const { return destination; }
		float getEnemySpeedMultiplier() const { return enemySpeedMultiplier; }
		bool getAimTowardsTarget() const { return activeTowardsTarget; }

	private:
		// Helper functions
		void setSpriteRotation() override;

		// Values
		sf::Vector2f targetPos;
		sf::Vector2f destination;
		sf::Vector2f direction;
		sf::Vector2f deflectionTarget;

		float enemySpeedMultiplier = 0.f;
		float broadsideDistance = 600.f;

		bool activeTowardsTarget = false;
	};
}