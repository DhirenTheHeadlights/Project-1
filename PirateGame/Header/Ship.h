#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

/// <summary>
/// This class is used to represent the player ship in the game.
/// </summary>

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
	~Ship();

	// Create the ship
	void createShip(ShipType type, ShipClass shipClass);

	// Movement functions
	void move(sf::Vector2f map);
	void direction(sf::Vector2f veclocity, float elapsed, sf::Vector2f map);
	void collisionMovement(int axis); 

	// Draw functions
	void draw(sf::Vector2f map);
	void drawVector(const sf::Vector2f& start, const sf::Vector2f& vector, sf::Color color = sf::Color::Red);

	// Setters
	void setPosition(sf::Vector2f pos) { ship.setPosition(pos); }
	void setHealth(float hp) { health = hp; }
	void setFriction(bool friction) { this->friction = friction;}

	// Getters
	sf::Vector2f getPosition();
	sf::Vector2f getSize() { return ship.getSize(); }
	sf::RectangleShape& getShape() { return ship; }
	sf::Vector2f getVelocity() { return velocity; }
	int getCollisionAxis() { return axis; }
	float getHealth() { return health; }
private:
	// Window reference
	sf::RenderWindow& window;

	// Clock reference
	sf::Clock deltaTime;

	// This will be implemented later
	sf::Sprite sprite;
	sf::Texture texture;

	// Temporary rectangle to represent the ship
	sf::RectangleShape ship;

	// Variables to store the ship's values
	float health;
	float maxHealth;
	float baseSpeed;
	float speed;
	sf::Vector2f velocity;
	float regenRate;
	float scaling = 1;

	// Variables to store the ship's position and last valid position
	sf::Vector2f position;
	sf::Vector2f lastValidPos;

	// Friction value and boolean and axis value
	float frictionCoeff = 0.1f;
	bool friction = false;
	int axis;

	// Rectangle shape for the health bar
	sf::RectangleShape healthBarGreen;
	sf::RectangleShape healthBarRed;

	// Clock for regenerating health
	sf::Clock healthRegenClock;
};

