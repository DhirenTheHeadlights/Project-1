#pragma once

/// Derived enemy movement handler class

#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class EnemyShipMovementHandler : public ShipMovementHandler {
	public:
		EnemyShipMovementHandler(sf::Sprite& sprite) : ShipMovementHandler(sprite) {};
		~EnemyShipMovementHandler() {};

		// Movement function
		void move(float baseSpeed, sf::Vector2f sailRotation) override;

		// Setters
		void setTargetPosition(sf::Vector2f targetPos) { this->targetPos = targetPos; }
		void setEnemySpeedMultiplier(float enemySpeedMultiplier) { this->enemySpeedMultiplier = enemySpeedMultiplier; }
		void setTurningSpeed(float turningSpeed) { this->turningSpeed = turningSpeed; }
		void setIsActiveTowardsTarget(bool active) { this->isActiveTowardsTarget = active; }
		void setDestination(sf::Vector2f destination) { this->destination = destination; }

		// Getters
		bool getIsActiveTowardsPlayer() const { return isActiveTowardsTarget; }
		sf::Vector2f getDestination() const { return destination; }
		float getEnemySpeedMultiplier() const { return enemySpeedMultiplier; }

	private:
		// Helper functions
		void setSpriteRotation() override;

		// Values
		sf::Vector2f targetPos;
		sf::Vector2f destination;

		float turningSpeed = 0.f;
		float enemySpeedMultiplier = 0.f;

		bool isActiveTowardsTarget = false;
	};
}