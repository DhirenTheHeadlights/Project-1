#pragma once

/// Derived enemy movement handler class

#include "ShipMovementHandler_PG.h"
#include "AstarAlgorithm_PG.h"

namespace PirateGame {
	class EnemyShipMovementHandler : public ShipMovementHandler {
	public:
		EnemyShipMovementHandler(sf::Sprite& sprite, float& baseSpeed, JSONLoader* json) : ShipMovementHandler(sprite, baseSpeed, json) {};
		~EnemyShipMovementHandler() {};

		// Movement function
		void update(const sf::Vector2f& sailRotation, const sf::Time& dt, const sf::Vector2f& windDirection, const float windSpeed) override;

		// Setters
		void setTargetPosition(sf::Vector2f& targetPos) { this->targetPos = targetPos; }
		void setEnemySpeedMultiplier(float enemySpeedMultiplier) { this->enemySpeedMultiplier = enemySpeedMultiplier; }
		void setDestination(const sf::Vector2f& destination) { 
			this->destination = destination;
			astar.setStartAndEndPoints(sprite.getPosition(), destination);
		}
		void setNearbySprites(const std::vector<sf::Sprite> nearbySprites) {
			this->nearbySprites = nearbySprites;
			astar.updateNearbySprites(nearbySprites);
		}
		void setActiveTowardsTarget(bool aimTowardsTarget) { this->activeTowardsTarget = aimTowardsTarget; }

		// Getters
		sf::Vector2f getDestination() const { return destination; }
		float getEnemySpeedMultiplier() const { return enemySpeedMultiplier; }
		bool getAimTowardsTarget() const { return activeTowardsTarget; }
		AStar& getAStar() { return astar; }

	private:
		// Helper functions
		void setSpriteRotation() override;

		AStar astar; // A* algorithm object

		std::vector<sf::Sprite> nearbySprites;

		// Values
		sf::Vector2f targetPos;
		sf::Vector2f destination;
		sf::Vector2f direction;
		sf::Vector2f deflectionTarget;

		float enemySpeedMultiplier = 0.5f;
		float broadsideDistance = 600.f;

		bool activeTowardsTarget = false;
	};
}