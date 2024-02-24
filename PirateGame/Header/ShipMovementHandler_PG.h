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
		sf::Vector2f move(float baseSpeed);
		void collisionMovement(sf::Sprite& collidingSprite);

		// Setters
		void setVelocity(sf::Vector2f velocity) { this->velocity = velocity; }
		void setIsColliding(bool isColliding) { this->isColliding = isColliding; }
		void setPosition(sf::Vector2f position) { sprite.setPosition(position); }

		// Getters
		sf::Vector2f getVelocity() { return velocity; }
		bool getIsColliding() { return isColliding; }
		sf::Vector2f getPosition() { return position; }

	private:
		// SFML Objects
		sf::RenderWindow* window = nullptr;
		sf::Clock deltaTime;
		sf::Sprite& sprite;

		sf::Vector2f velocity;
		sf::Vector2f position;

		bool isColliding = true;
		float rotation = 0;
		float speed = 0;
		float frictionCoefficient = 0.1f;

		// Helper functions
		sf::Vector2f normalize(sf::Vector2f vector);
		void updateVelocity(const sf::Vector2f& direction, float elapsedTime, const float baseSpeed);
		void applyBoundaryConstraints(sf::Vector2f& position, const sf::Vector2f& mapSize);
		void setSpriteRotation(sf::Vector2f& direction);
		float dot(const sf::Vector2f& v1, const sf::Vector2f& v2);
		void ensureSeparation(sf::Vector2f& position, const sf::Vector2f& normal, const sf::Sprite& collidingSprite);

	};
}

