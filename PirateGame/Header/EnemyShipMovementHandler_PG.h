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
		void setDestination(sf::Vector2f destination) { this->destination = destination; }
		void setActiveTowardsTarget(bool aimTowardsTarget) { this->activeTowardsTarget = aimTowardsTarget; }

		// Getters
		sf::Vector2f getDestination() const { return destination; }
		float getEnemySpeedMultiplier() const { return enemySpeedMultiplier; }
		bool getAimTowardsTarget() const { return activeTowardsTarget; }

	private:
		// Helper functions
		void setSpriteRotation() override;
		sf::Vector2f deflectTravelDirection(const std::vector<sf::Sprite>& sprites, const sf::Vector2f travelDirection, const float deflectionDistance);

		// Values
		sf::Vector2f targetPos;
		sf::Vector2f destination;
		sf::Vector2f deflectionTarget;

		float turningSpeed = 0.f;
		float enemySpeedMultiplier = 0.f;
		float broadsideDistance = 600.f;

		float islandDeflectionPaddingScale = 1.5f;
		float deflectionDistanceLandmass = 2000.f;
		float deflectionDistanceShip = 500.f;
		float deflectionVectorGrowth = 2.f;
		float dotProductThreshold = 0.1f;
		int deflectionVectorCheckInterval = 100;

		bool activeTowardsTarget = false;
		bool hasDeflectionTarget = false;
	};
}