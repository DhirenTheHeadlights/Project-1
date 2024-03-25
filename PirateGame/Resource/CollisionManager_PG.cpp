#include "CollisionManager_PG.h"

using namespace PirateGame;

void CollisionManager::handleCollisions() {
	// Grab global hashmaps
	Hashmap<EnemyShip>* shipHashmap = GlobalHashmapHandler::getInstance().getShipHashmap();
	Hashmap<LandMass>* landMassHashmap = GlobalHashmapHandler::getInstance().getLandMassHashmap();

	// Grab the nearby objects for the player ship
	std::set<LandMass*> nearbyLandMasses = landMassHashmap->findObjectsNearObject(playerShip);
	std::set<EnemyShip*> nearbyShips = shipHashmap->findObjectsNearObject(playerShip);
	std::set<Cannonball*> nearbyCannonballs = GlobalHashmapHandler::getInstance().getCannonballHashmap()->findObjectsNearObject(playerShip);

	// Vector to store colliding land masses
	std::vector<LandMass*> collidingLandMasses;
	collidingLandMasses.clear();

	// Check if the player is colliding with any of the nearby land masses
	for (auto& i : nearbyLandMasses) {
		if (pixelPerfectTest(playerShip->getSprite(), i->getSprite())) {

			playerShip->getMovementHandler().collisionMovement(i->getSprite());
			playerShip->damageShip(collisionDamagePerSecond * collidingLandMasses.size());

			collidingLandMasses.push_back(i);

			GlobalSoundManager::getInstance().playSound(SoundId::Bonk);
		}
		else {
			playerShip->getMovementHandler().setIsColliding(false);
		}
	}

	// Check if the player is colliding with any of the nearby ships
	for (auto& i : nearbyCannonballs) {
		if (i->getSprite().getGlobalBounds().intersects(playerShip->getSprite().getGlobalBounds())) {
			playerShip->damageShip(collisionDamagePerSecond * collidingLandMasses.size());
			i->setInactive();

			GlobalSoundManager::getInstance().playSound(SoundId::CannonImpact);
		}
	}

	// Grab the nearby landmasses and ships for each active ship
	for (auto& enemyShip : nearbyShips) {
		if (!enemyShip->isActive()) continue;

		std::set<LandMass*> nearbyLandmasses = landMassHashmap->findObjectsNearObject(enemyShip);
		std::set<EnemyShip*> nearbyShips = shipHashmap->findObjectsNearObject(enemyShip);

		std::vector<LandMass*> collidingLandMasses;
		collidingLandMasses.clear();

		std::vector<EnemyShip*> collidingShips;
		collidingShips.clear();

		// Check if the enemy ship is colliding with any of the nearby land masses
		for (auto& i : nearbyLandmasses) {
			if (pixelPerfectTest(enemyShip->getSprite(), i->getSprite())) {
				enemyShip->getMovementHandler().collisionMovement(i->getSprite());
				enemyShip->damageShip(collisionDamagePerSecond * collidingLandMasses.size());

				collidingLandMasses.push_back(i);
			}
			else {
				enemyShip->getMovementHandler().setIsColliding(false);
			}
		}

		// Check if the enemy ship is colliding with any of the nearby ships
		for (auto& i : nearbyShips) {
			if (shipCollisionTest(enemyShip, i)) {
				// Ignore the collision if the ships are the same
				if (enemyShip == i) continue;

				enemyShip->getMovementHandler().collisionMovement(i->getSprite());
				enemyShip->damageShip(collisionDamagePerSecond * collidingLandMasses.size());

				collidingShips.push_back(i);
			}
			else {
				enemyShip->getMovementHandler().setIsColliding(false);
			}
		}

		// Check if the player ship is colliding with any of the nearby enemy ships
		for (auto& i : nearbyShips) {
			if (shipCollisionTest(playerShip, i)) {
				playerShip->getMovementHandler().collisionMovement(i->getSprite());
				playerShip->damageShip(collisionDamagePerSecond * collidingLandMasses.size());

				collidingShips.push_back(i);
			}
			else {
				playerShip->getMovementHandler().setIsColliding(false);
			}
		}

		// Check if the enemy ship is colliding with any of the nearby cannonballs
		for (auto& i : nearbyCannonballs) {
			if (i->getSprite().getGlobalBounds().intersects(enemyShip->getSprite().getGlobalBounds())) {
				enemyShip->damageShip(collisionDamagePerSecond);
				i->setInactive();

				GlobalSoundManager::getInstance().playSound(SoundId::CannonImpact);
			}
		}
	}

}

// This pixel perfect test is specifically for ships and land masses
// This pixel perfect test is specifically for ships and land masses
bool CollisionManager::pixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2, unsigned alphaLimit) {
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
	float distance = sqrt((center2.x - center1.x) * (center2.x - center1.x) + (center2.y - center1.y) * (center2.y - center1.y));

	// Use an adjusted sum of semi-major axes as a simple overlap check
	// This factor can be tuned based on ship shapes/collision "sensitivity"
	float collisionDistance = (a1 + a2) * 0.75f; 

	// Check if the distance is less than the adjusted sum of semi-major axes
	if (distance < collisionDistance) {
		return true; // Collision detected
	}

	return false; // No collision detected
}