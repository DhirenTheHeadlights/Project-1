#pragma once

/// Derived enemy movement handler class

#include "ShipMovementHandler_PG.h"

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
		void setDestination(sf::Vector2f destination) { this->destination = destination; }
		void setActiveTowardsTarget(bool aimTowardsTarget) { this->activeTowardsTarget = aimTowardsTarget; }

		// Getters
		sf::Vector2f getDestination() const { return destination; }
		float getEnemySpeedMultiplier() const { return enemySpeedMultiplier; }
		bool getAimTowardsTarget() const { return activeTowardsTarget; }

	private:
		// Helper functions
		void setSpriteRotation() override;
		sf::Vector2f deflectTravelDirection(const std::vector<sf::Sprite>& sprites, const sf::Vector2f travelDirection, const float deflectionDistance, const float deflectionPaddingScale = 1.f);
		sf::Vector2f calculateDeflectionVector(const sf::FloatRect& deflectionBounds, const sf::Vector2f travelDirection, const float deflectionDistance);

		// Values
		sf::Vector2f targetPos;
		sf::Vector2f destination;
		sf::Vector2f deflectionTarget;

		sf::Sprite deflectionSprite;

		float enemySpeedMultiplier = 0.f;
		float broadsideDistance = 600.f;

		float islandDeflectionPaddingScaleMax = 2.f;  // To give some variation
		float islandDeflectionPaddingScaleMin = 1.2f; // To give some variation
		float shipDeflectionPaddingScale = 2.f;
		float deflectionDistanceLandmass = 2000.f;
		float deflectionDistanceShip = 1000.f;
		float deflectionVectorGrowth = 2.f;
		float dotProductThreshold = 0.1f;
		float closestCornerWeight = 0.8f;
		int deflectionVectorCheckInterval = 500;

		bool activeTowardsTarget = false;
		bool hasPickedFirstCorner = false;
	};
}