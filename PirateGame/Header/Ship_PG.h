#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

/// <summary>
/// This class is used to represent the player ship in the game.
/// </summary>


namespace PirateGame {

	// This enum will be used to determine the ship type
	enum class ShipType { Player, Enemy };

	// This enum will be used to determine the ship class
	enum class ShipClass {
		Sloop,
		Brigantine,
		Frigate,
		ManOWar,
		Galleon
	};

	class Ship {
	public:
		Ship(sf::RenderWindow& window);
		~Ship() {};

		// Create the ship
		void createShip(ShipType type, ShipClass shipClass);

		// Movement functions
		void move(sf::Vector2f map);
		void direction(sf::Vector2f veclocity, float elapsed, sf::Vector2f map);
		void collisionMovement(int axis);
		void stop();

		// Draw functions
		void draw(sf::Vector2f map);
		void drawVector(const sf::Vector2f& start, const sf::Vector2f& vector, sf::Color color = sf::Color::Red);

		// Setters
		void setPosition(sf::Vector2f pos) { sprite.setPosition(pos); }
		void setHealth(float hp) { health = hp; }
		void setFriction(bool friction) { this->friction = friction; }

		// Getters
		sf::Vector2f getSpritePosition();
		sf::Vector2f getVelocity() { return velocity; }
		int getCollisionAxis() { return axis; }
		float getHealth() { return health; }
		sf::Sprite& getSprite() { return sprite; }
	private:
		// SFML Objects
		sf::RenderWindow& window;
		sf::Clock deltaTime;
		sf::Sprite sprite;
		sf::Texture texture;

		// Variables to store the ship's values
		float health = 1;
		float maxHealth = 1;
		float baseSpeed = 1;
		float speed = 1;
		float regenRate = 1;
		float scaling = 5;
		float rotation = 0;

		sf::Vector2f velocity;
		sf::Vector2f constSpriteBounds;
		sf::Vector2f spriteOrigin = sf::Vector2f(123.f, 128.f);

		// Collision rectangle
		sf::RectangleShape collisionRect;

		// Variables to store the ship's position and last valid position
		sf::Vector2f position;
		sf::Vector2f lastValidPos;

		// Friction value and boolean and axis value
		float frictionCoeff = 0.1f;
		bool friction = false;
		int axis = 0;

		// Rectangle shape for the health bar
		sf::RectangleShape healthBarGreen;
		sf::RectangleShape healthBarRed;

		// Clock for regenerating health
		sf::Clock healthRegenClock;
	};

}
