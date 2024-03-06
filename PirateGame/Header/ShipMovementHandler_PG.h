#pragma once

/// <summary>
/// This class controls the movement and collisions of the ship class
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

#include "GlobalValues_PG.h"

namespace PirateGame {
	class ShipMovementHandler {
	public:
		ShipMovementHandler(sf::Sprite& sprite) : sprite(sprite) {};
		~ShipMovementHandler() {};

		// Movement functions
		void move(float baseSpeed);
		void collisionMovement(sf::Sprite& collidingSprite);
		void addCannonRecoil(sf::Vector2f direction, float recoil);

		// Setters
		void setVelocity(sf::Vector2f velocity) { this->velocity = velocity; }
		void setIsColliding(bool isColliding) { this->isColliding = isColliding; }
		void setPosition(sf::Vector2f position) { sprite.setPosition(position); }
		void setStopShipRotationFlag(bool stopShipRotationFlag) { this->stopShipRotationFlag = stopShipRotationFlag; }
		void setStopShipFlag(bool stopShipFlag) { this->stopShipFlag = stopShipFlag; }

		// Getters
		sf::Vector2f getVelocity() const { return velocity; }
		bool getIsColliding() const { return isColliding; }
		sf::Vector2f getPosition() const { return sprite.getPosition(); }
		bool getStopShipRotationFlag() const { return stopShipRotationFlag; }
		bool getStopShipFlag() const { return stopShipFlag; }
		float getSpeed() const { return speed; }

	private:
		// SFML Objects
		sf::RenderWindow* window = nullptr;
		sf::Clock deltaTime;
		sf::Sprite& sprite;

		sf::Vector2f velocity;
		sf::Vector2f position;

		bool isColliding = true;
		bool stopShipRotationFlag = false;
		bool stopShipFlag = false;

		float rotation = 0;
		float speed = 0;
		float baseSpeed = 0;
		float frictionCoefficient = 0.1f; // For friction
		float dampingFactor = 0.5f; // For collisionMovement
		float separationDistance = 5.0f; // For collisionMovement
		float pushOutDistance = 1.0f; // For ensureSeparation
		float turningSpeed = 0.1f; // For setSpriteRotation

		const float pi = 3.14159265359f;

		// Helper functions
		sf::Vector2f normalize(sf::Vector2f vector);
		void updateVelocity(const sf::Vector2f& direction, float elapsedTime, const float baseSpeed);
		void applyBoundaryConstraints(sf::Vector2f& position, const sf::Vector2f& mapSize);
		void setSpriteRotation(sf::Vector2f& direction);
		float dot(const sf::Vector2f& v1, const sf::Vector2f& v2);
		void ensureSeparation(sf::Vector2f& position, const sf::Vector2f& normal, const sf::Sprite& collidingSprite);

	};
}

