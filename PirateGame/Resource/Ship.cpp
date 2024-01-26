#include "Ship.h"
#include <cmath>

// Constructor
Ship::Ship(sf::RenderWindow& window) : window(window) {
	position = sf::Vector2f(0, 0);
}

// Destructor
Ship::~Ship() {
	
}

// Create the ship
void Ship::createShip(ShipType type, ShipClass level) {
	// This switch case will determine the ship type 
	// and assign the appropriate texture/sprite

	switch (level) {
	case ShipClass::Sloop:
		// Load the values
		speed = 100.f;
		health = 100;
		regenRate = 1;

		// Load the texture
		if (!texture.loadFromFile("PirateGameSprites/pg_ship_sloop.png")) std::cout << "Failed to load Sloop texture." << std::endl;
		sprite.setTexture(texture);
		sprite.setOrigin(103.f, 273.f);
		sprite.setScale(.1f * scaling, .1f * scaling);
		break;
	case ShipClass::Brigantine:
		// Load the values
		speed = 95.f;
		health = 133;
		regenRate = 1.48;
		regenRate = 1.48;

		// Load the texture
		if (!texture.loadFromFile("PirateGameSprites/pg_ship_brigantine.png")) std::cout << "Failed to load Brigantine texture." << std::endl;
		sprite.setTexture(texture);
		sprite.setOrigin(117.f, 292.f);
		sprite.setScale(.12f * scaling, .12f * scaling);
		break;
	case ShipClass::Frigate:
		// Load the values
		speed = 82.f;
		health = 192;
		regenRate = 2.15;

		// Load the texture
		if (!texture.loadFromFile("PirateGameSprites/pg_ship_frigate.png")) std::cout << "Failed to load Frigate texture." << std::endl;
		sprite.setTexture(texture);
		sprite.setOrigin(145.f, 339.f);
		sprite.setScale(.15f * scaling, .15f * scaling);
		break;
	case ShipClass::ManOWar:
		// Load the values
		speed = 77.f;
		health = 250;
		regenRate = 3;

		// Load the texture
		if (!texture.loadFromFile("PirateGameSprites/pg_ship_manowar.png")) std::cout << "Failed to load Man O'War texture." << std::endl;
		sprite.setTexture(texture);
		sprite.setOrigin(135.f, 388.f);
		sprite.setScale(.18f * scaling, .18f * scaling);
		break;
	case ShipClass::Galleon:
		// Load the values
		speed = 63.f;
		health = 380;
		regenRate = 4.6;

		// Load the texture
		if (!texture.loadFromFile("PirateGameSprites/pg_ship_galleon.png")) std::cout << "Failed to load Galleon texture." << std::endl;
		sprite.setTexture(texture);
		sprite.setOrigin(209.f, 452.f);
		sprite.setScale(.23f * scaling, .23f * scaling);
		break;
	default:
		// Handle other cases
		break;
	}

	// This switch case will determine the ship class
	// and assign the appropriate texture/sprite
	switch (type) {
	case ShipType::Player:
		// Load the texture and values
		break;
	case ShipType::Enemy:
		// Load the texture and values
		break;
	}

	// Set the base speed
	baseSpeed = 10 * speed; // Temporary speed up for testing

	// Set the maximum health
	maxHealth = health;

	// For now, we will just use a rectangle to represent the ship
	ship.setSize(sf::Vector2f(100, 100));
	ship.setFillColor(sf::Color::Green);
}

// Move the ship
void Ship::move(sf::Vector2f map) {
	speed = baseSpeed;

	// Get the position of the mouse
	sf::Vector2f viewPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// Get the direction to the mouse
	sf::Vector2f dirToMouse = viewPos - getPosition();

	// Get the elapsed time
	float elapsed = deltaTime.restart().asSeconds();

	// Normalize the direction
	if (dirToMouse.x == 0 && dirToMouse.y == 0) return; // Return if the mouse is already on the ship.
	float length = sqrt(dirToMouse.x * dirToMouse.x + dirToMouse.y * dirToMouse.y);
	dirToMouse.x /= length;
	dirToMouse.y /= length;

	// If friction is enabled, decrease the speed of a specific axis
	if (friction) {
		speed = baseSpeed * frictionCoeff;
		if (axis == 0) velocity.y = dirToMouse.y * speed;
		else if (axis == 1) velocity.x = dirToMouse.x * speed;
	}
	else velocity = sf::Vector2f(dirToMouse.x * speed, dirToMouse.y * speed);
	
	// Rotate the sprite using conversion from vector to angle with atan2
	// Calculate the angle and convert it to degrees
	float angleRad = std::atan2(dirToMouse.y, dirToMouse.x);
	const float pi = 3.1415926f;
	float angleDeg = angleRad * 180.0f / pi;

	// In SFML, 0 degrees is along the positive X-axis.
	// Rotate the sprite to align with the direction to the mouse
	sprite.setRotation(angleDeg + 90); // Adding 90 degrees if the sprite's up is its top

	// Draw the velocity vector
	drawVector(getPosition(), velocity, sf::Color::Red);

	// Move the ship
	direction(velocity, elapsed, map);
}

// Get the direction of the ship
void Ship::direction(sf::Vector2f velocity, float elapsed, sf::Vector2f map) {
	// Set the last valid position
	lastValidPos = position;

	// Update the position
	position.x += velocity.x * elapsed; 
	position.y += velocity.y * elapsed;

	// Boundary checks
	if (position.x < 0) position.x = 0;
	if (position.y < 0) position.y = 0;
	float size = ship.getSize().x;
	if (position.x > map.x) position.x = map.x - size;
	if (position.y > map.x) position.y = map.x - size;

	// Set the position of the ship
	float x = position.x - size;
	float y = position.y - size;
	ship.setPosition(x, y);
	sprite.setPosition(x + size * 0.5, y + size * 0.5);
}

// Draw the ship
void Ship::draw(sf::Vector2f map) {
	// Set the health to 0 if it is negative
	if (health < 0) health = 0;

	// Regen health every 250 milliseconds
	if (health < maxHealth && health >= 0) {
		if (healthRegenClock.getElapsedTime().asMilliseconds() > 250) {
			health += regenRate;
			healthRegenClock.restart();
		}
	}

	// Determine the size of the health bar green based on health
	healthBarGreen.setSize(sf::Vector2f(100 * health / maxHealth, 10));
	healthBarGreen.setFillColor(sf::Color::Green);

	// Set the position centered below the ship
	healthBarGreen.setPosition(ship.getPosition().x, ship.getPosition().y + ship.getSize().y + 10);

	// Determine the size of the health bar red based on health
	healthBarRed.setSize(sf::Vector2f(100, 10));
	healthBarRed.setFillColor(sf::Color::Red);

	// Set the position centered below the ship
	healthBarRed.setPosition(ship.getPosition().x, ship.getPosition().y + ship.getSize().y + 10);

	// Draw the health bars
	window.draw(healthBarRed);
	window.draw(healthBarGreen);

	// Draw the ship
	move(map);
	window.draw(ship);
	window.draw(sprite);
}

// Get the ship's position as the center of the ship
sf::Vector2f Ship::getPosition() {
return sf::Vector2f(ship.getPosition().x + ship.getSize().x / 2, ship.getPosition().y + ship.getSize().y / 2);
}

// Apply collision movement to the ship; 0 = x-axis, 1 = y-axis
void Ship::collisionMovement(int axis) {
	// Set the axis
	this->axis = axis;

	// Set one of the positions to the last valid position
	if (axis == 0) {
		position.x = lastValidPos.x;
		velocity.x = 0;
	}
	else if (axis == 1) {
		position.y = lastValidPos.y;
		velocity.y = 0;
	}

	// Set the position of the ship
	float size = ship.getSize().x;
	ship.setPosition(position.x - size, position.y - size);
}

// Draw a vector
void Ship::drawVector(const sf::Vector2f& start, const sf::Vector2f& vector, sf::Color color) {
	// Create a VertexArray with two points
	sf::VertexArray line(sf::Lines, 2);

	// Set the position of the first point to the starting point
	line[0].position = start;

	// Set the position of the second point to the end of the vector
	line[1].position = start + vector;

	// Set the color of the line
	line[0].color = color;
	line[1].color = color;

	// Draw the line
	window.draw(line);
}
