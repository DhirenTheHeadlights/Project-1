#include "LandMassHandler_PG.h"

using namespace PirateGame;

LandMassHandler::~LandMassHandler() {
	for (auto& landMass : landmasses) {
		GlobalHashmapHandler::getInstance().getLandMassHashmap()->removeObject(landMass.get());
	}
}

// Add all the land masses to the vector
void LandMassHandler::addLandMasses(int numLandMassesPerChunk, float minDistBetweenLandmasses) {
	// Grab all chunks
	std::vector<Map*> maps = GlobalMap::getInstance().getAllChunks();

	for (auto& map : maps) {
		addLandMassesToChunk(*map, numLandMassesPerChunk, minDistBetweenLandmasses);
	}
}

void LandMassHandler::addLandMassesToChunk(Map& map, int numLandMasses, float minDistBetweenLandmasses) {
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
	std::shared_ptr<LandMass> landMass = std::make_unique<LandMass>();
	landMass->createLandMass(type);
	landMass->setPosition(position);


	GlobalHashmapHandler::getInstance().getLandMassHashmap()->addObject(landMass.get());
	landmasses.push_back(std::move(landMass));
}

// Draw all the land masses
void LandMassHandler::drawLandMasses() {
	// Draw all the land masses and add them to the hashmap
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	for (auto& i : landmasses) {
		i->draw(*window);
	}
}

void LandMassHandler::interactWithLandmasses(PlayerShip* ship) {
	if (nearestLandMass == nullptr) {
		std::set<LandMass*> nearbyLandMasses = GlobalHashmapHandler::getInstance().getLandMassHashmap()->findObjectsNearObject(ship, interactionDistance);

		for (auto& landMass : nearbyLandMasses) {
			sf::Vector2f shipPosition = ship->getSprite().getPosition(); // Ship position is already the center of the sprite
			sf::Vector2f landMassPosition = landMass->getSprite().getPosition() + landMass->getSprite().getGlobalBounds().getSize() / 2.f; // Land mass position is the top left corner of the sprite
			float distance = vm::magnitude(shipPosition - landMassPosition);

			if (distance <= interactionDistance && landMass->getType() == LandMassType::Island) {
				// Set this as the nearest land mass
				nearestLandMass = landMass;
				break; // Stop checking for other islands
			}
		}

		// Reset the 'player said no' and enteredIsland flag for all islands not nearby
		for (auto& landMass : landmasses) {
			if (landMass->getType() == LandMassType::Island && nearbyLandMasses.find(landMass.get()) == nearbyLandMasses.end()) {
				landMass->getIslandMenu()->setEnteredIsland(false);
				landMass->getIslandMenu()->setHasPlayerSaidNo(false);
			}
		}
	}
	// Draw the island menu for the nearest land mass
	else {
		nearestLandMass->getIslandMenu()->setShip(*ship);
		nearestLandMass->getIslandMenu()->draw();
		nearestLandMass->getIslandMenu()->update();
	}

	// Set the nearest land mass to null if the player is no longer near it
	if (nearestLandMass != nullptr) {
		sf::Vector2f shipPosition = ship->getSprite().getPosition(); // Ship position is already the center of the sprite
		sf::Vector2f landMassPosition = nearestLandMass->getSprite().getPosition() + nearestLandMass->getSprite().getGlobalBounds().getSize() / 2.f; // Land mass position is the top left corner of the sprite
		float distance = vm::magnitude(shipPosition - landMassPosition);

		if (distance > interactionDistance) {
			nearestLandMass = nullptr;
		}
	}
}