#pragma once

/// This class controls the movement and collisions of the ship class

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

#include "VectorMath.h"

#include "AstarAlgorithm_PG.h"

namespace PirateGame {
	class ShipMovementHandler {
	public:
		ShipMovementHandler(sf::Sprite& sprite, float& baseSpeed) : sprite(sprite), baseSpeed(baseSpeed) {};
		~ShipMovementHandler() {};

		// Movement functions 
		virtual void update(const sf::Vector2f& sailDirection, const sf::Time& dt, const sf::Vector2f& windDirection, const float windSpeed) = 0;
		void move(const sf::Vector2f& sailDirection, const sf::Time& dt, const sf::Vector2f& windDirection, const float windSpeed);
		void rotateTowards(float targetAngle);
		void collisionMovement(const sf::Sprite& collidingSprite);
		void addCannonRecoil(const sf::Vector2f& direction, const float recoil);

		virtual void updateVelocity(const sf::Vector2f& direction, const sf::Time& elapsedTime, const sf::Vector2f& sailDirection, const sf::Vector2f& windDirection, const float windSpeed);
		virtual void setSpriteRotation() = 0;

		// These functions stay constant
		void ensureSeparation(const sf::Vector2f& normal, const sf::Sprite& collidingSprite);

		// Setters
		void setSpeed(float speed) { this->speed = speed; } // For testing purposes, but cannot be set above the baseSpeed
		void setTurningSpeed(float turningSpeed) { this->turningSpeed = turningSpeed; }
		void setTurningMultiplier(float turningMultiplier) { this->turningMultiplier = turningMultiplier; }
		void setIsColliding(bool isColliding) { this->isColliding = isColliding; }
		void setStopShipRotationFlag(bool stopShipRotationFlag) { this->stopShipRotationFlag = stopShipRotationFlag; }
		void setAnchorDrop(bool anchorDrop) { this->dropAnchor = anchorDrop; }

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

		float turningSpeed = 0.1f;
		float turningMultiplier = 1.f;
		float speed = 0;
		float speedBeforeAnchorDrop = 0;
		float frictionCoefficient = 0.1f; // For friction
		float dampingFactor = 0.5f;		  // For collisionMovement
		float separationDistance = 5.0f;  // For collisionMovement
		float pushOutDistance = 1.0f;	  // For ensureSeparation

	protected:
		sf::Sprite& sprite;
	};
}

