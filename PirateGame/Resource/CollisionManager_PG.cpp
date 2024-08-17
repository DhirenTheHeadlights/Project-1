#include "CollisionManager_PG.h"

using namespace PirateGame;

void CollisionManager::handleCollisions(GlobalQuadtreeHandler* GQH) {
	// Handle collisions for the player ship
	handleShipCollisions(GQH, playerShip);

	std::erase_if(ships, [](const std::shared_ptr<EnemyShip>& s) { return s->getIsDead(); });

	// Handle collisions for each enemy ship
	for (auto& enemyShip : ships) {
		enemyShip->getMovementHandler()->setNearbySprites(handleShipCollisions(GQH, enemyShip.get()));
	}

	// Grab nearby cannonballs for each cannonball
	std::vector<Cannonball*> cannonballs = GQH->getCannonballQuadtree()->getObjects();

	for (auto& c1 : cannonballs) {
		std::vector<Cannonball*> nearbyCannonballs = GQH->getCannonballQuadtree()->findObjectsNearObject(c1, nearbyDistanceCannonball);
		for (auto& c2 : nearbyCannonballs) {
			// Skip if the cannonballs are the same cannonball
			if (c1->getID() == c2->getID()) continue;
			handleCannonballCollision(c1, c2);
		}
	}
}

std::vector<sf::Sprite> CollisionManager::handleShipCollisions(GlobalQuadtreeHandler* GQH, Ship* ship) {
	std::vector<LandMass*> nearbyLandMasses = GQH->getLandMassQuadtree()->findObjectsNearObject(ship, nearbyDistanceLandmass);
	std::vector<EnemyShip*> nearbyShips = GQH->getEnemyShipQuadtree()->findObjectsNearObject(ship, nearbyDistanceShip);
	std::vector<Cannonball*> nearbyCannonballs = GQH->getCannonballQuadtree()->findObjectsNearObject(ship, nearbyDistanceCannonball);

	// Vectors to hold the colliding objects
	std::vector<LandMass*> collidingLandMasses = {};
	std::vector<Ship*> collidingShips = {};
	std::vector<Cannonball*> collidingCannonballs = {};

	// Check if the ship is colliding with any of the nearby land masses and send to movement handler
	std::vector<sf::Sprite> nearbySprites;
	for (auto& i : nearbyLandMasses) {
		handleLandMassCollision(ship, i, collidingLandMasses);
		nearbySprites.push_back(i->getSprite());
	}

	// Check if the ship is colliding with any of the nearby ships
	for (auto& i : nearbyShips) {
		// Skip if the ship is the same ship
		if (i->getID() == ship->getID()) continue;
		handleShipCollision(ship, i, collidingShips);
		nearbySprites.push_back(i->getSprite());
	}

	// Check if the ship is colliding with any of the nearby cannonballs 
	for (auto& i : nearbyCannonballs) {
		// Skip if the cannonball is from the same ship
		if (i->getShipID() == ship->getID()) continue;
		handleCannonballCollision(ship, i, collidingCannonballs);
	}

	// Return the nearby sprites for the ship's movement handler if needed
	return nearbySprites;
}

void CollisionManager::handleLandMassCollision(Ship* ship, LandMass* landmass, std::vector<LandMass*>& collidingLandMasses) {
	// Check if the ship is colliding with the land mass
	if (pixelPerfectTest(ship, landmass)) {
		// Move the ship away from the land mass
		ship->getMovementHandler()->collisionMovement(landmass->getSprite());
		collidingLandMasses.push_back(landmass);

		// Damage the ship based on the multiplier
		ship->damageShip(collisionDamagePerFrame * collidingLandMasses.size());

		// Play the collision sound
		context.GSM->playSound(SoundId::Bonk);
	}
	else {
		ship->getMovementHandler()->setIsColliding(false);
	}
}

void CollisionManager::handleShipCollision(Ship* ship1, Ship* ship2, std::vector<Ship*>& collidingShips) {
	// Check if the ships are colliding
	if (shipCollisionTest(ship1, ship2)) {
		// Add the ship to the colliding ships vector
		collidingShips.push_back(ship1);
		collidingShips.push_back(ship2);

		// Move the ships away from each other
		ship1->getMovementHandler()->collisionMovement(ship2->getSprite());
		ship2->getMovementHandler()->collisionMovement(ship1->getSprite());

		// Damage the ships based on the multiplier
		ship1->damageShip(collisionDamagePerFrame * collidingShips.size());
		ship2->damageShip(collisionDamagePerFrame * collidingShips.size());

		// Play the collision sound
		context.GSM->playSound(SoundId::Bonk);
	}
	else {
		ship1->getMovementHandler()->setIsColliding(false);
		ship2->getMovementHandler()->setIsColliding(false);
	}
}

void CollisionManager::handleCannonballCollision(Ship* ship, Cannonball* cannonball, std::vector<Cannonball*>& collidingCannonballs) {
	// Check if the ship is colliding with the cannonball
	if (!cannonball->getSprite().getGlobalBounds().intersects(ship->getSprite().getGlobalBounds())) return;

	const sf::Sprite& sprite1 = ship->getSprite();
	const sf::Sprite& sprite2 = cannonball->getSprite();

	// Calculate the positions of the ship's center
	sf::Vector2f center1 = sprite1.getPosition();

	// Calculate the semi-major (a) and semi-minor (b) axes for the ship
	// Assuming the ship's bounding box width is the semi-major axis and height is the semi-minor axis
	float a1 = sprite1.getGlobalBounds().width / 2.0f;
	float b1 = sprite1.getGlobalBounds().height / 2.0f;

	// Calculate the distance between the center of the ship and the cannonball
	float distance = vm::distance(center1, sprite2.getPosition());

	// Use an adjusted sum of semi-major axes as a simple overlap check
	// This factor can be tuned based on ship shapes/collision "sensitivity"
	float collisionDistance = (a1 + sprite2.getGlobalBounds().width / 2.0f) * 0.75f;

	if (distance < collisionDistance) {
		// Damage the ship based on the multiplier
		collidingCannonballs.push_back(cannonball);
		ship->damageShip(collisionDamagePerFrame * collidingCannonballs.size());
		cannonball->setInactive();

		// Check if the cannonball is from the player ship and the ship dies from it
		addedExp = false;
		if (cannonball->getShipID() == playerShip->getID() && ship->getHealth() <= 0.001 && !addedExp) {
			playerShip->addExperience(killExp);
			addedExp = true;
		}
	}
}

// Collision check for cannonballs with each other
void CollisionManager::handleCannonballCollision(Cannonball* c1, Cannonball* c2) {
	// Check if the cannonballs are colliding
	if (!c1->getSprite().getGlobalBounds().intersects(c2->getSprite().getGlobalBounds())) return;

	sf::CircleShape circle1(c1->getSprite().getGlobalBounds().width / 2.0f);
	circle1.setPosition(c1->getSprite().getPosition());
	sf::CircleShape circle2(c2->getSprite().getGlobalBounds().width / 2.0f);
	circle2.setPosition(c2->getSprite().getPosition());

	// Calculate the distance between the centers of the two cannonballs
	float distance = vm::distance(circle1.getPosition(), circle2.getPosition());
	if (distance < circle1.getRadius() + circle2.getRadius()) {
		// Set both cannonballs to inactive
		c1->setInactive();
		c2->setInactive();
	}
}

// This collision check is specifically for ships and other ships
bool CollisionManager::shipCollisionTest(Ship* ship1, Ship* ship2) {
	// Check for global bounds intersection as a preliminary fast reject
	if (!ship1->getSprite().getGlobalBounds().intersects(ship2->getSprite().getGlobalBounds())) return false;

	// Get sprite references for ease of use
	const sf::Sprite& sprite1 = ship1->getSprite();
	const sf::Sprite& sprite2 = ship2->getSprite();

	// Calculate the positions of the ships' centers
	sf::Vector2f center1 = sprite1.getPosition();
	sf::Vector2f center2 = sprite2.getPosition();

	// Calculate the semi-major (a) and semi-minor (b) axes for both ships
	// Assuming the ships' bounding box width is the semi-major axis and height is the semi-minor axis
	float a1 = sprite1.getGlobalBounds().width / 2.0f;
	float b1 = sprite1.getGlobalBounds().height / 2.0f;
	float a2 = sprite2.getGlobalBounds().width / 2.0f;
	float b2 = sprite2.getGlobalBounds().height / 2.0f;

	// Calculate the distance between the centers of the two ships
	float distance = vm::distance(center1, center2);

	// Use an adjusted sum of semi-major axes as a simple overlap check
	// This factor can be tuned based on ship shapes/collision "sensitivity"
	float collisionDistance = (a1 + a2) * 0.75f;

	// Check if the distance is less than the adjusted sum of semi-major axes
	if (distance < collisionDistance) {
		return true; // Collision detected
	}

	return false; // No collision detected
}

// This pixel perfect test is specifically for ships and land masses. Sprite1 is the ship, sprite2 is the land mass
bool CollisionManager::pixelPerfectTest(Ship* ship, LandMass* landmass, unsigned alphaLimit) {
	const sf::Sprite& sprite1 = ship->getSprite();
	const sf::Sprite& sprite2 = landmass->getSprite();

	sf::RenderWindow& window = *context.GV->getWindow();
	sf::FloatRect intersection;
	if (!sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds(), intersection)) return false;

	const sf::Texture* texture1 = sprite1.getTexture();
	const sf::Texture* texture2 = sprite2.getTexture();
	if (!texture1 || !texture2) return false;

	const sf::Image& image1 = context.GTH->getShipTextures().getShipTextureManager().getImage(ship->getShipClass());
	const sf::Image& image2 = landmass->getImage(context);

	const sf::Uint8* pixels1 = image1.getPixelsPtr();
	const sf::Uint8* pixels2 = image2.getPixelsPtr();

	sf::IntRect rect1 = sprite1.getTextureRect();
	sf::IntRect rect2 = sprite2.getTextureRect();

	// Assuming sails make up about 20% of the sprite width on each side
	float sailExclusionFactor = 0.8f; // Use 80% of the width for the body

	// Semi-major axis (long axis) adjusted to be thinner
	float a = (rect1.width * sailExclusionFactor) / 2.0f;
	float b = rect1.height / 2.0f; // Semi-minor axis remains the same

	for (int i = static_cast<int>(intersection.left); i < intersection.left + intersection.width; i++) {
		for (int j = static_cast<int>(intersection.top); j < intersection.top + intersection.height; j++) {
			// Convert global coordinates to sprite1's local space
			sf::Vector2f localPos1 = sprite1.getInverseTransform().transformPoint(static_cast<float>(i), static_cast<float>(j));

			// Adjust for sail exclusion by re-centering the ellipse in the reduced width
			float x_rel = localPos1.x - (rect1.width * (1 - sailExclusionFactor) / 2.0f) - (a * 2 * sailExclusionFactor) / 2.0f;
			float y_rel = localPos1.y - rect1.height / 2.0f;
			if ((x_rel * x_rel) / (a * a) + (y_rel * y_rel) / (b * b) <= 1) {
				// Now check pixel alpha values for collision
				int idx1 = ((int)localPos1.y * rect1.width + (int)localPos1.x) * 4;
				// Convert global coordinates to sprite2's local space
				sf::Vector2f localPos2 = sprite2.getInverseTransform().transformPoint(static_cast<float>(i), static_cast<float>(j));
				int idx2 = ((int)localPos2.y * rect2.width + (int)localPos2.x) * 4;

				if (idx1 < 0 || idx1 >= rect1.width * rect1.height * 4 || idx2 < 0 || idx2 >= rect2.width * rect2.height * 4) {
					// Skip this pixel as it's out of the valid range
					continue;
				}

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
