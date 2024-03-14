#include "LandMassHandler_PG.h"

using namespace PirateGame;

LandMassHandler::~LandMassHandler() {
	for (auto& landMass : landMasses) {
		hashmap.removeLandMass(landMass);
		delete landMass;
	}
}

// Add all the land masses to the vector
void LandMassHandler::addLandMasses(int numLandMasses, float minDistBetweenLandmasses) {
	// Grab a numLandMasses number of points from the map
	std::vector<sf::Vector2f> points = map.getRandomPositions(minDistBetweenLandmasses, numLandMasses);

	for (int i = 0; i < points.size(); i++) {
		// Generate a random number between 0 and 2
		int randNum = rand() % 2;

		// Create a land mass based on the random number; proportional to the distance between land masses
		if (randNum == 0) createLandmass(LandMassType::Island, points[i]);
		else if (randNum == 1) createLandmass(LandMassType::Rock, points[i]);
		else createLandmass(LandMassType::Shipwreck, points[i]);
	}
}

void LandMassHandler::createLandmass(LandMassType type, sf::Vector2f position) {
	LandMass* landMass = new LandMass();
	landMass->createLandMass(type, texture);

	landMass->setPosition(position);
	landMasses.push_back(landMass);
	hashmap.addLandMass(landMass);
}

// Draw all the land masses
void LandMassHandler::drawLandMasses() {
	// Draw all the land masses and add them to the hashmap
	for (auto& i : landMasses) {
		i->draw(*window);
	}
}

void LandMassHandler::interactWithLandmasses(PlayerShip& ship) {
    std::set<LandMass*> nearbyLandMasses = hashmap.findLandMassNearPlayer(ship, *window);

    handleCollisions(ship, nearbyLandMasses);
    
    for (auto& landMass : nearbyLandMasses) {
        sf::Vector2f shipPosition = ship.getSprite().getPosition(); // Ship position is already the center of the sprite
        sf::Vector2f landMassPosition = sf::Vector2f(landMass->getSprite().getPosition().x + landMass->getSprite().getGlobalBounds().width / 2, landMass->getSprite().getPosition().y + landMass->getSprite().getGlobalBounds().height / 2);
        float distance = sqrt(pow(shipPosition.x - landMassPosition.x, 2.f) + pow(shipPosition.y - landMassPosition.y, 2.f));

        if (distance <= interactionDistance && landMass->getType() == LandMassType::Island) {
            // Prompt the player to open the market here
            openMarket(ship, landMass);
            break; // Stop checking for other islands
        }
    }

    // Reset the 'player said no' and enteredIsland flag for all islands not nearby
    for (auto& landMass : landMasses) {
        if (landMass->getType() == LandMassType::Island && nearbyLandMasses.find(landMass) == nearbyLandMasses.end()) {
            landMass->getIslandMenu()->setEnteredIsland(false);
			landMass->getIslandMenu()->setHasPlayerSaidNo(false);
        }
    }
}

void LandMassHandler::openMarket(PlayerShip& ship, LandMass* landMass) {
	landMass->getIslandMenu()->setShip(ship);
	landMass->getIslandMenu()->draw();
}

void LandMassHandler::handleCollisions(PlayerShip& ship, std::set<LandMass*> nearbyLandMasses){
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
			ship.damageShip(collisionDamagePerSecond * collidingLandMasses.size());
		}
	}

	// Play the collision sound while the player is colliding with a land mass
	if (isColliding) GlobalSoundManager::getInstance().playSound(SoundId::Bonk);
	else ship.getMovementHandler().setIsColliding(false);
}

// Pixel perfect collision detection
bool LandMassHandler::pixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2, unsigned alphaLimit) {
	sf::RenderWindow& window = *GlobalValues::getInstance().getWindow();
	sf::FloatRect intersection;
	if (!sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds(), intersection)) return false;

	const sf::Texture* texture1 = sprite1.getTexture();
	const sf::Texture* texture2 = sprite2.getTexture();
	if (!texture1 || !texture2) return false;

	sf::Image image1 = texture1->copyToImage(); // Costly operation, but works because there are only
	sf::Image image2 = texture2->copyToImage(); // A few collisions per frame at most
	const sf::Uint8* pixels1 = image1.getPixelsPtr();
	const sf::Uint8* pixels2 = image2.getPixelsPtr();

	sf::IntRect rect1 = sprite1.getTextureRect();
	sf::IntRect rect2 = sprite2.getTextureRect();

	// Ellipse parameters in sprite1's local coordinate space
	float a = rect1.width / 2.0f; // Semi-major axis without sails
	float b = rect1.height / 2.0f; // Semi-minor axis

	for (int i = static_cast<int>(intersection.left); i < intersection.left + intersection.width; i++) {
		for (int j = static_cast<int>(intersection.top); j < intersection.top + intersection.height; j++) {
			// Convert global coordinates to sprite1's local space
			sf::Vector2f localPos1 = sprite1.getInverseTransform().transformPoint(static_cast<float>(i), static_cast<float>(j));

			// Check if the point is inside the ellipse defined by sprite1's local bounds
			float x_rel = localPos1.x - rect1.width / 2.0f;
			float y_rel = localPos1.y - rect1.height / 2.0f;
			if ((x_rel * x_rel) / (a * a) + (y_rel * y_rel) / (b * b) <= 1) {
				// Now check pixel alpha values for collision
				int idx1 = ((int)localPos1.y * rect1.width + (int)localPos1.x) * 4;
				// Convert global coordinates to sprite2's local space
				sf::Vector2f localPos2 = sprite2.getInverseTransform().transformPoint(static_cast<float>(i), static_cast<float>(j));
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
