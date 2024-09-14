#pragma once

/// This class controls the movement and collisions of the ship class

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include "VectorMath.h"
#include "AstarAlgorithm_PG.h"'
#include "JSONLoader_PG.h"

namespace PirateGame {
	class ShipMovementHandler {
	public:
		ShipMovementHandler(sf::Sprite& sprite, float& baseSpeed, JSONLoader* json) : sprite(sprite), baseSpeed(baseSpeed), json(json) {
			this->turningSpeed = json->getGameData().gameConfig.shipData.turningSpeed;
			this->turningMultiplier = json->getGameData().gameConfig.shipData.turningMultiplier;
		};
		~ShipMovementHandler() {};

		// Movement functions 
		virtual void update(const sf::Vector2f& sailDirection, const sf::Time& dt, const sf::Vector2f& windDirection, const float windSpeed) = 0;
		void move(const sf::Vector2f& sailDirection, const sf::Time& dt, const sf::Vector2f& windDirection, const float windSpeed);
		void rotateTowards(float targetAngle) const;
		void collisionMovement(const sf::Sprite& collidingSprite);
		void addCannonRecoil(const sf::Vector2f& direction, const float recoil);

		virtual void updateVelocity(const sf::Vector2f& direction, const sf::Time& elapsedTime, const sf::Vector2f& sailDirection, const sf::Vector2f& windDirection, const float windSpeed);
		virtual void setSpriteRotation() = 0;

		// These functions stay constant
		void ensureSeparation(const sf::Vector2f& normal, const sf::Sprite& collidingSprite) const;

		// Setters
		void setSpeed(const float speed) { this->speed = speed; } // For testing purposes, but cannot be set above the baseSpeed
		void setIsColliding(const bool isColliding) { this->isColliding = isColliding; }
		void setStopShipRotationFlag(const bool stopShipRotationFlag) { this->stopShipRotationFlag = stopShipRotationFlag; }
		void setAnchorDrop(const bool anchorDrop) { this->dropAnchor = anchorDrop; }
		void setTurningSpeed(float turningSpeed) { this->turningSpeed = turningSpeed; }
		void setTurningMultiplier(float turningMultiplier) { this->turningMultiplier = turningMultiplier; }
		// Getters
		float getSpeed() const { return speed; }
		sf::Vector2f getVelocity() const { return velocity; }

		bool getStopShipRotationFlag() const { return stopShipRotationFlag; }
		bool getDroppedAnchor() const { return dropAnchor; }
		bool getIsColliding() const { return isColliding; }

	private:
		sf::Vector2f velocity;
		sf::Vector2f initialPosition;

		bool isColliding = false;
		bool stopShipRotationFlag = false;
		bool dropAnchor = false;
		bool anchorPushBack = false;

		float& baseSpeed;
		float turningMultiplier = 0;
		float turningSpeed = 0;

		float speed = 0;
		float speedBeforeAnchorDrop = 0;



	protected:
		sf::Sprite& sprite;
		JSONLoader* json = nullptr;
	};
}

