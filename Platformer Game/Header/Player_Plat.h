#pragma once
#include "SFML/Graphics.hpp"
#include "Textures_Plat.h"
#include <iostream>


namespace PlatformerGame {
	class Player {
	public:
		Player(sf::Vector2f& map, sf::RenderWindow& window);

		// Movement methods
		void move();
		void draw();

		// Physics methods
		void applyGravity();

		//State methods
		void handlePlayerState();

		//Collision methods
		void checkPlatformCollision(const std::vector<sf::RectangleShape>& platforms);

		sf::Vector2f getPosition() const { return playerSprite.getPosition(); }


	private:
		// SFML variables
		sf::Clock frametimeClock;
		sf::RenderWindow& window;

		// Physics variables
		sf::Vector2f velocity;
		sf::Vector2f accel;
		sf::Vector2f maxVelocity = sf::Vector2f(30.f, 30.f);  // Maximum velocity (x, y)
		sf::Vector2f position;  // Position of the player
		sf::Vector2f spriteScale = sf::Vector2f(2, 2);  // Scale of the player sprite
		sf::Vector2f prevPosition;  // Previous position of the player

		static const sf::Vector2f GRAVITY;  // Gravity constant


		bool isOnGround = false;                    // Track if the player is on the ground
		bool isMoving = false;						// Track is player is moving side to side
		bool isCollided = false;					// Track if the player is colliding with a platform

		// Boundary variables
		float leftBoundary;
		float rightBoundary;
		float topBoundary;
		float bottomBoundary;

		float elapsed = 0;
		float scaling = 1;

		Textures texture;
		sf::Texture playerTexture;
		sf::Sprite playerSprite;

	};
}