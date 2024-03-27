#pragma once

/// Derived enemy movement handler class

#include "ShipMovementHandler_PG.h"

namespace PirateGame {
	class EnemyShipMovementHandler : public ShipMovementHandler {
	public:
		EnemyShipMovementHandler(sf::Sprite& sprite) : ShipMovementHandler(sprite) {};
		~EnemyShipMovementHandler() {};

		// Movement function
		void move(float baseSpeed) override;

		// Setters
		void setPlayerPosition(sf::Vector2f playerPos) { this->playerPos = playerPos; }
		void setPlayerVelocity(sf::Vector2f playerVelocity) { this->playerVelocity = playerVelocity; }
		void setEnemySpeedMultiplier(float enemySpeedMultiplier) { this->enemySpeedMultiplier = enemySpeedMultiplier; }
		void setTurningSpeed(float turningSpeed) { this->turningSpeed = turningSpeed; }
		void setIsActiveTowardsPlayer(bool active) { this->isActiveTowardsPlayer = active; }
		void setDestination(sf::Vector2f destination) { this->destination = destination; }

		// Getters
		bool getIsActiveTowardsPlayer() { return isActiveTowardsPlayer; }
		sf::Vector2f getDestination() { return destination; }

	private:
		// Helper functions
		void setSpriteRotation() override;

		// Values
		sf::Vector2f playerPos;
		sf::Vector2f destination;
		sf::Vector2f playerVelocity;
		float turningSpeed = 0.f;
		float enemySpeedMultiplier = 0.f;
		bool isActiveTowardsPlayer = false;
	};
}