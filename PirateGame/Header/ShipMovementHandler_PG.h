#pragma once

/// <summary>
/// This class controls the movement and collisions of the ship class
/// </summary>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "GlobalValues_PG.h"

namespace PirateGame {
	class ShipMovementHandler {
	public:
		ShipMovementHandler(sf::Sprite& sprite) : sprite(sprite) {};
		~ShipMovementHandler() {};

		// Movement functions
		sf::Vector2f move(float speed);

		// Setters
		void setVelocity(sf::Vector2f velocity) { this->velocity = velocity; }
		void setPosition(sf::Vector2f position) { this->position = position; }
		void setCollisionMovement(bool isColliding, sf::Vector2f normal = sf::Vector2f(0, 0)) { 
			this->isColliding = isColliding;
			this->normal = normal;
		}

		// Getters
		sf::Vector2f getVelocity() { return velocity; }

	private:
		// Movement functions
		void normalizeDirection(sf::Vector2f& direction);
		void adjustVelocity(sf::Vector2f& dirToMouse, float baseSpeed);
		void applyCollisionAdjustments();
		void applyRotation(const sf::Vector2f& dirToMouse);
		sf::Vector2f adjustPositionAndBoundaryChecks(sf::Vector2f velocity, float deltaTime, sf::Vector2f map);
		void constrainPositionToBounds(sf::Vector2f map);

		// SFML Objects
		sf::RenderWindow* window = nullptr;
		sf::Clock deltaTime;
		sf::Sprite& sprite;

		// Movement variables
		sf::Vector2f velocity;
		sf::Vector2f position;
		sf::Vector2f normal;

		float rotation;
		float frictionSpeedCoeff = 0.1f;
		float friction_factor = 0.000007f;
		float speed = 1000;

		bool isColliding = false;
	};
}

