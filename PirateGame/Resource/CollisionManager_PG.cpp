#include "CollisionManager_PG.h"

#include "QuadtreeHandler_PG.h"

using namespace PirateGame;

void CollisionManager::handleCollisions() {
	// Handle collisions for the player ship
	handleShipCollisions(playerShip);

	std::erase_if(ships, [](const std::shared_ptr<EnemyShip>& s) { return s->getIsDead(); });

	// Handle collisions for each enemy ship
	for (auto& enemyShip : ships) {
		enemyShip->getMovementHandler()->setNearbySprites(handleShipCollisions(enemyShip.get()));
	}

	// Grab nearby cannonballs for each cannonball

	for (const std::vector<Cannonball*> cannonballs = QuadtreeHandler::cannonballQuadtree->getObjects(); auto& c1 : cannonballs) {
		for (std::vector<Cannonball*> nearbyCannonballs = QuadtreeHandler::cannonballQuadtree->findObjectsNearObject(c1, nearbyDistanceCannonball); const auto& c2 : nearbyCannonballs) {
			// Skip if the cannonballs are the same cannonball
			if (c1 == c2) continue;
			handleCannonballCollision(c1, c2);
		}
	}
}

std::vector<sf::Sprite> CollisionManager::handleShipCollisions(Ship* ship) {
	const std::vector<LandMass*> nearbyLandMasses = QuadtreeHandler::landmassQuadtree->findObjectsNearObject(ship, nearbyDistanceLandmass);
	const std::vector<EnemyShip*> nearbyShips = QuadtreeHandler::enemyShipQuadtree->findObjectsNearObject(ship, nearbyDistanceShip);
	const std::vector<Cannonball*> nearbyCannonballs = QuadtreeHandler::cannonballQuadtree->findObjectsNearObject(ship, nearbyDistanceCannonball);

	// Vectors to hold the colliding objects
	std::vector<LandMass*> collidingLandMasses = {};
	std::vector<Ship*> collidingShips = {};
	std::vector<Cannonball*> collidingCannonballs = {};

	// Check if the ship is colliding with any of the nearby land masses and send to movement handler
	std::vector<sf::Sprite> nearbySprites;
	for (const auto& i : nearbyLandMasses) {
		handleLandMassCollision(ship, i, collidingLandMasses);
		nearbySprites.push_back(i->getSprite());
	}

	// Check if the ship is colliding with any of the nearby ships
	for (const auto& i : nearbyShips) {
		// Skip if the ship is the same ship
		if (i->getID() == ship->getID()) continue;
		handleShipCollision(ship, i, collidingShips);
		nearbySprites.push_back(i->getSprite());
	}

	// Check if the ship is colliding with any of the nearby cannonballs 
	for (const auto& i : nearbyCannonballs) {
		// Skip if the cannonball is from the same ship
		if (i->shipID == ship->getID()) continue;
		handleCannonballCollision(ship, i, collidingCannonballs);
	}

	// Return the nearby sprites for the ship's movement handler if needed
	return nearbySprites;
}

void CollisionManager::handleLandMassCollision(Ship* ship, LandMass* landmass, std::vector<LandMass*>& collidingLandMasses) const {
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

void CollisionManager::handleShipCollision(Ship* ship1, Ship* ship2, std::vector<Ship*>& collidingShips) const {
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
	if (!cannonball->sprite.getGlobalBounds().intersects(ship->getSprite().getGlobalBounds())) return;

	const sf::Sprite& sprite1 = ship->getSprite();
	const sf::Sprite& sprite2 = cannonball->sprite;

	// Calculate the positions of the ship's center
	const sf::Vector2f center1 = sprite1.getPosition();

	// Calculate the semi-major (a) and semi-minor (b) axes for the ship
	// Assuming the ship's bounding box width is the semi-major axis and height is the semi-minor axis
	const float a1 = sprite1.getGlobalBounds().width / 2.0f;
	float b1 = sprite1.getGlobalBounds().height / 2.0f;

	// Calculate the distance between the center of the ship and the cannonball
	const float distance = vm::distance(center1, sprite2.getPosition());

	// Use an adjusted sum of semi-major axes as a simple overlap check
	// This factor can be tuned based on ship shapes/collision "sensitivity"

	if (const float collisionDistance = (a1 + sprite2.getGlobalBounds().width / 2.0f) * 0.75f; distance < collisionDistance) {
		// Damage the ship based on the multiplier
		collidingCannonballs.push_back(cannonball);
		ship->damageShip(collisionDamagePerFrame * collidingCannonballs.size());
		cannonball->isActive = false;

		// Check if the cannonball is from the player ship and the ship dies from it
		addedExp = false;
		if (cannonball->shipID == playerShip->getID() && ship->getHealth() <= 0.001 && !addedExp) {
			playerShip->addExperience(killExp);
			addedExp = true;
		}
	}
}

// Collision check for cannonballs with each other
void CollisionManager::handleCannonballCollision(Cannonball* c1, Cannonball* c2) {
	// Check if the cannonballs are colliding
	if (!c1->sprite.getGlobalBounds().intersects(c2->sprite.getGlobalBounds())) return;

	sf::CircleShape circle1(c1->sprite.getGlobalBounds().width / 2.0f);
	circle1.setPosition(c1->sprite.getPosition());
	sf::CircleShape circle2(c2->sprite.getGlobalBounds().width / 2.0f);
	circle2.setPosition(c2->sprite.getPosition());

	// Calculate the distance between the centers of the two cannonballs
	float distance = vm::distance(circle1.getPosition(), circle2.getPosition());
	if (distance < circle1.getRadius() + circle2.getRadius()) {
		// Set both cannonballs to inactive
		c1->isActive = false;
		c2->isActive = false;
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
	const sf::Vector2f center1 = sprite1.getPosition();
	const sf::Vector2f center2 = sprite2.getPosition();

	// Calculate the semi-major (a) and semi-minor (b) axes for both ships
	// Assuming the ships' bounding box width is the semi-major axis and height is the semi-minor axis
	const float a1 = sprite1.getGlobalBounds().width / 2.0f;
	float b1 = sprite1.getGlobalBounds().height / 2.0f;
	const float a2 = sprite2.getGlobalBounds().width / 2.0f;
	float b2 = sprite2.getGlobalBounds().height / 2.0f;

	// Calculate the distance between the centers of the two ships
	const float distance = vm::distance(center1, center2);

	// Use an adjusted sum of semi-major axes as a simple overlap check
	// This factor can be tuned based on ship shapes/collision "sensitivity"

	// Check if the distance is less than the adjusted sum of semi-major axes
	if (const float collisionDistance = (a1 + a2) * 0.75f; distance < collisionDistance) {
		return true; // Collision detected
	}

	return false; // No collision detected
}

// This pixel perfect test is specifically for ships and land masses. Sprite1 is the ship, sprite2 is the land mass
bool CollisionManager::pixelPerfectTest(Ship* ship, LandMass* landmass, const unsigned alphaLimit) const {
	const sf::Sprite& sprite1 = ship->getSprite();
	const sf::Sprite& sprite2 = landmass->getSprite();

	sf::RenderWindow& window = *context.GV->getWindow();
	sf::FloatRect intersection;
	if (!sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds(), intersection)) return false;

	const sf::Texture* texture1 = sprite1.getTexture();
	const sf::Texture* texture2 = sprite2.getTexture();
	if (!texture1 || !texture2) return false;

	const sf::Image& image1 = context.GTH->getShipTextures().getShipTextureManagerByRegion(ship->getBirthRegion()).getImage(ship->getShipClass());
	const sf::Image& image2 = landmass->getImage(context);

	const sf::Uint8* pixels1 = image1.getPixelsPtr();
	const sf::Uint8* pixels2 = image2.getPixelsPtr();

	const sf::IntRect rect1 = sprite1.getTextureRect();
	const sf::IntRect rect2 = sprite2.getTextureRect();

	// Assuming sails make up about 20% of the sprite width on each side
	constexpr float sailExclusionFactor = 0.8f; // Use 80% of the width for the body

	// Semi-major axis (long axis) adjusted to be thinner
	const float a = (rect1.width * sailExclusionFactor) / 2.0f;
	const float b = rect1.height / 2.0f; // Semi-minor axis remains the same

	for (int i = static_cast<int>(intersection.left); i < intersection.left + intersection.width; i++) {
		for (int j = static_cast<int>(intersection.top); j < intersection.top + intersection.height; j++) {
			// Convert global coordinates to sprite1's local space
			const sf::Vector2f localPos1 = sprite1.getInverseTransform().transformPoint(static_cast<float>(i), static_cast<float>(j));

			// Adjust for sail exclusion by re-centering the ellipse in the reduced width
			const float x_rel = localPos1.x - (rect1.width * (1 - sailExclusionFactor) / 2.0f) - (a * 2 * sailExclusionFactor) / 2.0f;
			const float y_rel = localPos1.y - rect1.height / 2.0f;
			if ((x_rel * x_rel) / (a * a) + (y_rel * y_rel) / (b * b) <= 1) {
				// Now check pixel alpha values for collision
				const int idx1 = (static_cast<int>(localPos1.y) * rect1.width + static_cast<int>(localPos1.x)) * 4;
				// Convert global coordinates to sprite2's local space
				const sf::Vector2f localPos2 = sprite2.getInverseTransform().transformPoint(static_cast<float>(i), static_cast<float>(j));
				const int idx2 = (static_cast<int>(localPos2.y) * rect2.width + static_cast<int>(localPos2.x)) * 4;

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
