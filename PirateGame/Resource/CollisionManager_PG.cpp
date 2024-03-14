#include "CollisionManager_PG.h"

using namespace PirateGame;

void CollisionManager::addObjectsToHashmaps() {
	// Add landmasses to hashmap
	for (auto& landmass : landMasses) {
		landMassHashmap.addLandMass(landmass);
	}

	// Add ships to hashmap
	/*for (auto& enemyShip : enemyShips) {
		shipHashmap.addShip(enemyShip);
	}*/
}

void CollisionManager::handleCollisions() {
	// Grab the nearby landmasses and ships for the player ship
	std::set<Landmass*> nearbyLandMasses = landMassHashmap.findLandMassNearShip(playerShip);
	//std::set<Ship*> nearbyShips = shipHashmap.findShipsNearShip(playerShip);

	// Vector to store colliding land masses
	std::vector<Landmass*> collidingLandMasses;
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

	//// Grab the nearby landmasses and ships for each active ship
	//for (auto& enemyShip : enemyShips) {
	//	if (!enemyShip->isActive()) continue;

	//	std::set<Landmass*> nearbyLandmasses = landMassHashmap.findLandMassNearShip(enemyShip);
	//	std::set<Ship*> nearbyShips = shipHashmap.findShipsNearShip(enemyShip);

	//	std::vector<Landmass*> collidingLandMasses;
	//	collidingLandMasses.clear();

	//	// Check if the enemy ship is colliding with any of the nearby land masses
	//	for (auto& i : nearbyLandmasses) {
	//		if (pixelPerfectTest(enemyShip->getSprite(), i->getSprite())) {
	//			enemyShip->getMovementHandler().collisionMovement(i->getSprite());
	//			enemyShip->damageShip(collisionDamagePerSecond * collidingLandMasses.size());

	//			collidingLandMasses.push_back(i);
	//		}
	//	}
	//}

}

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