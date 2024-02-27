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
		void applyGravity(); // Apply gravity to the player

		//State methods
		void handlePlayerState();

		sf::Vector2f getPosition() const { return player.getPosition(); }


	private:
		// SFML variables
		sf::Clock frametimeClock;
		sf::RenderWindow& window;

		// Physics variables
		sf::Vector2f velocity;
		sf::Vector2f accel;
		sf::Vector2f maxVelocity = sf::Vector2f(30.f, 30.f);  // Maximum velocity (x, y)
		sf::Vector2f position;  // Position of the player

		static const sf::Vector2f GRAVITY;  // Gravity constant


		bool isOnGround = false;                    // Track if the player is on the ground
		bool isMoving = false;						// Track is player is moving side to side

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