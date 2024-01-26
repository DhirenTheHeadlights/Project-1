#include "LandMassHandler_PG.h"

LandMassHandler::LandMassHandler(Map& map) : hashmap(map), map(map), soundManager(soundFile) {
}

LandMassHandler::~LandMassHandler() {
	for (auto& landMass : landMasses) {
		hashmap.removeLandMass(landMass);
		delete landMass;
	}
}

// Add all the land masses to the vector
void LandMassHandler::addLandMasses(int numLandMasses, float minDistBetweenLandmasses) {
	for (int i = 0; i < numLandMasses; i++) {
		LandMass* landMass = new LandMass();

		// Generate a random number between 0 and 10
		int randNum = rand() % 10;

		// If the number is between 0 and 6, generate a rock
		if (randNum < 6) {
			landMass->createLandMass(LandMassType::Rock, texture);

			// Generate a random position for the land mass
			auto randPos = map.getRandomPosition(minDistBetweenLandmasses);

			// Set the position of the land mass if there is a valid value
			if (randPos.has_value()) {
				landMass->setPosition(randPos.value());

				// Add the land mass to the vector
				landMasses.push_back(landMass);
			}
			else {
				delete landMass;
				continue;
			}
		}
		// If the number is between 6 and 8, generate a island
		else if (randNum < 8) {
			landMass->createLandMass(LandMassType::Island, texture);

			// Generate a random position for the land mass
			auto randPos = map.getRandomPosition(5.f * minDistBetweenLandmasses);

			// Set the position of the land mass if there is a valid value
			if (randPos.has_value()) {
				landMass->setPosition(randPos.value());

				// Add the land mass to the vector
				landMasses.push_back(landMass);
			}
			else {
				delete landMass;
				continue;
			}
		}
		// If the number is between 8 and 10, generate a shipwreck
		else {
			landMass->createLandMass(LandMassType::Shipwreck, texture);

			// Generate a random position for the land mass
			auto randPos = map.getRandomPosition(2.f * minDistBetweenLandmasses);

			// Set the position of the land mass if there is a valid value
			if (randPos.has_value()) {
				landMass->setPosition(randPos.value());

				// Add the land mass to the vector
				landMasses.push_back(landMass);
			}
			else {
				delete landMass;
				continue;
			}
		}
	}

	std::cout << "Number of land masses: " << landMasses.size() << std::endl;
}

// Draw all the land masses
void LandMassHandler::drawLandMasses(sf::RenderWindow& window, Ship& ship) {
	// Draw all the land masses and add them to the hashmap
	for (auto& i : landMasses) {
		i->draw(window);
		hashmap.addLandMass(i);
	}

	// Handle the collisions between the player and the land masses
	handleCollisions(ship, window);
}

// Handle the collision between the player ship and the land masses
void LandMassHandler::handleCollisions(Ship& ship, sf::RenderWindow& window) {
	// Get the nearby land masses
	std::set<LandMass*> nearbyLandMasses = hashmap.findLandMassNearPlayer(ship, window);

	// Vector to store colliding land masses
	std::vector<LandMass*> collidingLandMasses;
	collidingLandMasses.clear();

	// Boolean to check if the player is colliding with a land mass
	bool isColliding = false;

	// Check if the player is colliding with any of the nearby land masses
	for (auto& i : nearbyLandMasses) {

		// If the player is colliding with a land mass
		if (ship.getShape().getGlobalBounds().intersects(i->getShape().getGlobalBounds())) {

			// Determine the collision axis
			int collisionAxis = determineCollisionAxis(ship, i);

			// Apply friction to the player
			ship.collisionMovement(collisionAxis);
			ship.setFriction(true);

			// If the player is colliding with a rock, stop the player from moving
			if (i->getType() == LandMassType::Rock) {
			}

			// If the player is colliding with a island, stop the player from moving
			else if (i->getType() == LandMassType::Island) {
			}

			// If the player is colliding with a shipwreck, stop the player from moving
			else if (i->getType() == LandMassType::Shipwreck) {
			}

			// Set the boolean to true
			isColliding = true;

			// Add the land mass to the vector of colliding land masses
			collidingLandMasses.push_back(i);

			// If the player collides, decrease the health of the player, multiplied by the number of colliding land masses
			ship.setHealth(ship.getHealth() - 0.2f * collidingLandMasses.size());
		}

		// Draw the bounds of the land mass
		drawBounds(window, i->getShape());

		// Draw the bounds of the player
		drawBounds(window, ship.getShape());
	}

	// Play the collision sound while the player is colliding with a land mass
	if (isColliding) {
		soundManager.playSound();
	}

	// If the player is not colliding with a land mass, set the friction to false
	else {
		ship.setFriction(false);
	}
}

// Determine the collision axis
int LandMassHandler::determineCollisionAxis(Ship& ship, LandMass* landMass) {
	// Get the bounds of the player and the land mass
	sf::FloatRect shipBounds = ship.getShape().getGlobalBounds();
	sf::FloatRect landMassBounds = landMass->getShape().getGlobalBounds();

	// Get the center of the player and the land mass
	sf::Vector2f shipCenter = sf::Vector2f(shipBounds.left + shipBounds.width / 2, shipBounds.top + shipBounds.height / 2);
	sf::Vector2f landMassCenter = sf::Vector2f(landMassBounds.left + landMassBounds.width / 2, landMassBounds.top + landMassBounds.height / 2);

	// Get the distance between the centers of the player and the land mass
	sf::Vector2f distance = shipCenter - landMassCenter;

	// Define a thereshold value that determines if the axis flips (for corner collisions)
	const float threshold = 10.f;

	// Calculate the difference between the distances
	float diff = abs(abs(distance.x) - abs(distance.y));

	// Check if the difference is significant enough to determine a clear axis
	if (diff > threshold) {
		return (abs(distance.x) > abs(distance.y)) ? 0 : 1;
	}
	
	// Or, if the difference is not significant enough, return existing collision axis
	else {
		return ship.getCollisionAxis();
	}
}

void LandMassHandler::drawBounds(sf::RenderWindow& window, const sf::RectangleShape& rect) {
	sf::FloatRect bounds = rect.getGlobalBounds();
	sf::RectangleShape frame(sf::Vector2f(bounds.width, bounds.height));
	frame.setPosition(bounds.left, bounds.top);
	frame.setFillColor(sf::Color::Transparent);
	frame.setOutlineColor(sf::Color::Yellow);
	frame.setOutlineThickness(5.f);
	window.draw(frame);
}