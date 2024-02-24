#include "LandMassHandler_PG.h"

using namespace PirateGame;

LandMassHandler::LandMassHandler() : hashmap(), soundManager(soundFile) {
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
}

// Draw all the land masses
void LandMassHandler::drawLandMasses(Ship& ship) {
	// Draw all the land masses and add them to the hashmap
	for (auto& i : landMasses) {
		i->draw(*window);
		hashmap.addLandMass(i);
	}

	// Handle the collisions between the player and the land masses
	handleCollisions(ship);
}

// Handle the collision between the player ship and the land masses
void LandMassHandler::handleCollisions(Ship& ship) {
	// Get the nearby land masses
	std::set<LandMass*> nearbyLandMasses = hashmap.findLandMassNearPlayer(ship, *window);

	// Vector to store colliding land masses
	std::vector<LandMass*> collidingLandMasses;
	collidingLandMasses.clear();

	// Boolean to check if the player is colliding with a land mass
	bool isColliding = false;

	// Check if the player is colliding with any of the nearby land masses
	for (auto& i : nearbyLandMasses) {
		if (pixelPerfectTest(ship.getSprite(), i->getSprite())) {

			// Collision movement for the ship
			ship.getMovementHandler().collisionMovement(i->getSprite());

			// Set the boolean to true
			isColliding = true;

			// Add the land mass to the vector of colliding land masses
			collidingLandMasses.push_back(i);

			// If the player collides, decrease the health of the player, multiplied by the number of colliding land masses
			ship.damageShip(0.2f * collidingLandMasses.size());
		}
	}

	// Play the collision sound while the player is colliding with a land mass
	if (isColliding) soundManager.playSound();
	else ship.getMovementHandler().setIsColliding(false);
}

// Pixel perfect collision detection
bool LandMassHandler::pixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2, unsigned alphaLimit, sf::RenderWindow& window, bool debug) {
	sf::FloatRect intersection;
	if (!sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds(), intersection)) return false;

	const sf::Texture* texture1 = sprite1.getTexture();
	const sf::Texture* texture2 = sprite2.getTexture();
	if (!texture1 || !texture2) return false;

	sf::Image image1 = texture1->copyToImage();
	sf::Image image2 = texture2->copyToImage();
	const sf::Uint8* pixels1 = image1.getPixelsPtr();
	const sf::Uint8* pixels2 = image2.getPixelsPtr();

	sf::IntRect rect1 = sprite1.getTextureRect();
	sf::IntRect rect2 = sprite2.getTextureRect();

	// Ellipse parameters in sprite1's local coordinate space
	float a = (rect1.width) / 2.0f; // Semi-major axis without sails
	float b = rect1.height / 2.0f; // Semi-minor axis

	for (int i = static_cast<int>(intersection.left); i < intersection.left + intersection.width; i++) {
		for (int j = static_cast<int>(intersection.top); j < intersection.top + intersection.height; j++) {
			// Convert global coordinates to sprite1's local space
			sf::Vector2f localPos1 = sprite1.getInverseTransform().transformPoint(i, j);

			// Check if the point is inside the ellipse defined by sprite1's local bounds
			float x_rel = localPos1.x - rect1.width / 2.0f;
			float y_rel = localPos1.y - rect1.height / 2.0f;
			if ((x_rel * x_rel) / (a * a) + (y_rel * y_rel) / (b * b) <= 1) {
				// Now check pixel alpha values for collision
				int idx1 = ((int)localPos1.y * rect1.width + (int)localPos1.x) * 4;
				// Convert global coordinates to sprite2's local space
				sf::Vector2f localPos2 = sprite2.getInverseTransform().transformPoint(i, j);
				int idx2 = ((int)localPos2.y * rect2.width + (int)localPos2.x) * 4;

				if (idx1 >= 0 && idx1 < rect1.width * rect1.height * 4 && idx2 >= 0 && idx2 < rect2.width * rect2.height * 4) {
					if (pixels1[idx1 + 3] > alphaLimit && pixels2[idx2 + 3] > alphaLimit) {
						return true; // Collision detected
					}
				}
			}
		}
	}

	return false; // No collision detected
}
