#pragma once

/// This class controls the movement and collisions of the ship class

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

#include "GlobalValues_PG.h"
#include "GlobalMap_PG.h"
#include "GlobalWindController_PG.h"

namespace PirateGame {
	class ShipMovementHandler {
	public:
		ShipMovementHandler(sf::Sprite& sprite) : sprite(sprite) {};
		~ShipMovementHandler() {};

		// Movement functions 
		virtual void move(float baseSpeed) = 0;
		void collisionMovement(sf::Sprite& collidingSprite);
		void addCannonRecoil(sf::Vector2f direction, float recoil);

		virtual void updateVelocity(const sf::Vector2f& direction, float elapsedTime, const float baseSpeed);
		virtual void setSpriteRotation() = 0;

		// These functions stay constant
		sf::Vector2f normalize(sf::Vector2f vector);
		float dot(const sf::Vector2f& v1, const sf::Vector2f& v2);
		void ensureSeparation(sf::Vector2f& position, const sf::Vector2f& normal, const sf::Sprite& collidingSprite);
		void setInitialPosition();

		// Setters
		void setVelocity(sf::Vector2f velocity) { this->velocity = velocity; }
		void setIsColliding(bool isColliding) { this->isColliding = isColliding; }
		void setPosition(sf::Vector2f position) { sprite.setPosition(position); }
		void setInitialPositionVariable(sf::Vector2f initialPosition) { this->initialPosition = initialPosition; }
		void setStopShipRotationFlag(bool stopShipRotationFlag) { this->stopShipRotationFlag = stopShipRotationFlag; }
		void setAnchorDrop(bool anchorDrop) { this->dropAnchor = anchorDrop; }
		void setBaseSpeed(float baseSpeed) { this->baseSpeed = baseSpeed; }
		void setWindSpeedApplied(bool windSpeedApplied) { this->windSpeedApplied = windSpeedApplied; }
		void setInitialPositionSet(bool initialPositionSet) { this->initialPositionSet = initialPositionSet; }

		// Getters
		float getSpeed() const { return speed; }
		float getBaseSpeed() const { return baseSpeed; }

		sf::Sprite& getSprite() { return sprite; }
		sf::Clock& getDeltaTime() { return deltaTime; }

		sf::Vector2f getVelocity() const { return velocity; }
		sf::Vector2f getInitialPosition() const { return initialPosition; }

		bool getStopShipRotationFlag() const { return stopShipRotationFlag; }
		bool getDroppedAnchor() const { return dropAnchor; }
		bool getIsColliding() const { return isColliding; }
		bool getWindSpeedApplied() const { return windSpeedApplied; }
		bool getInitialPositionSet() const { return initialPositionSet; }

	private:
		// SFML Objects
		sf::RenderWindow* window = nullptr;
		sf::Clock deltaTime;
		sf::Sprite& sprite;

		sf::Vector2f velocity;
		sf::Vector2f initialPosition;

		bool isColliding = false;
		bool stopShipRotationFlag = false;
		bool dropAnchor = false;
		bool anchorPushBack = false;
		bool windSpeedApplied = false;
		bool initialPositionSet = false;

		float rotation = 0;
		float speed = 0;
		float speedBeforeAnchorDrop = 0;
		float baseSpeed = 0;
		float frictionCoefficient = 0.1f; // For friction
		float dampingFactor = 0.5f;		  // For collisionMovement
		float separationDistance = 5.0f;  // For collisionMovement
		float pushOutDistance = 1.0f;	  // For ensureSeparation
		
	protected:
		const float pi = 3.14159265359f;
		sf::Vector2f position;
	};
}

