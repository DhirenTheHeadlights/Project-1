#pragma once
#include "SFML/Graphics.hpp"

class Player_Plat {
public:
	Player_Plat(sf::Vector2f& map, sf::RenderWindow& window);

	// Movement methods
	void move();
	void jump();
	void draw();

	// Physics methods
	void applyGravity();

	//State methods
	void handlePlayerState();
	

private:
	sf::RectangleShape player;
	sf::Vector2f position;
	sf::Clock frametimeClock;
	float elapsed;
	sf::RenderWindow& window;

	// Physics variables
	sf::Vector2f velocity;
	sf::Vector2f accel;
	sf::Vector2f maxVelocity;
	static const sf::Vector2f GRAVITY;  // Gravity constant
	bool isOnGround;                    // Track if the player is on the ground
	bool isMoving;						// Track is player is moving side to side
	float leftBoundary;
	float rightBoundary;
	float topBoundary;
	float bottomBoundary;
};

