#include "LandMassHandler_PG.h"

using namespace PirateGame;

LandMassHandler::~LandMassHandler() {
	for (auto& landMass : landmasses) {
		GlobalQuadtreeHandler::getInstance().getLandMassQuadtree()->removeObject(landMass.get());
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
		// Generate a random number between 0 and 3
		int randNum = rand() % 3;

		// Create a land mass based on the random number; proportional to the distance between land masses
		if (randNum == 0) createIsland(points[i]);
		else if (randNum == 1) createRock(points[i]);
		else createShipwreck(points[i]);
	}
}

void LandMassHandler::createIsland(sf::Vector2f position) {
	std::shared_ptr<Island> island = std::make_shared<Island>();
	island->getSprite().setPosition(position);
	island->createLandMass();
	landmasses.push_back(island);
	islands.push_back(island);
	GlobalQuadtreeHandler::getInstance().getLandMassQuadtree()->addObject(island.get());
	GlobalQuadtreeHandler::getInstance().getIslandQuadtree()->addObject(island.get());
}

void LandMassHandler::createRock(sf::Vector2f position) {
	std::shared_ptr<Rock> rock = std::make_shared<Rock>();
	rock->getSprite().setPosition(position);
	rock->createLandMass();
	landmasses.push_back(rock);
	rocks.push_back(rock);
	GlobalQuadtreeHandler::getInstance().getLandMassQuadtree()->addObject(rock.get());
	GlobalQuadtreeHandler::getInstance().getRockQuadtree()->addObject(rock.get());
}

void LandMassHandler::createShipwreck(sf::Vector2f position) {
	std::shared_ptr<Shipwreck> shipwreck = std::make_shared<Shipwreck>();
	shipwreck->getSprite().setPosition(position);
	shipwreck->createLandMass();
	landmasses.push_back(shipwreck);
	shipwrecks.push_back(shipwreck);
	GlobalQuadtreeHandler::getInstance().getLandMassQuadtree()->addObject(shipwreck.get());
	GlobalQuadtreeHandler::getInstance().getShipwreckQuadtree()->addObject(shipwreck.get());
}

// Draw all the land masses
void LandMassHandler::drawLandMasses() {
	// Remove any inactive land masses
	for (auto& ship : shipwrecks) {
		if (!ship->isActive()) {
			GlobalQuadtreeHandler::getInstance().getShipwreckQuadtree()->removeObject(ship.get());
			GlobalQuadtreeHandler::getInstance().getLandMassQuadtree()->removeObject(ship.get());
		}
	}
	std::erase_if(shipwrecks, [](std::shared_ptr<Shipwreck> ship) { return !ship->isActive(); });
	std::erase_if(landmasses, [](std::shared_ptr<LandMass> landmass) { return !landmass->isActive(); });

	// Draw all the land masses and add them to the hashmap
	sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
	for (auto& i : landmasses) {
		i->draw(*window);
	}
}

void LandMassHandler::interactWithLandmasses() {
	std::vector<Island*> nearbyIslands = GlobalQuadtreeHandler::getInstance().getIslandQuadtree()->findObjectsNearObject(playerShip, interactionDistance);
	std::vector<Shipwreck*> nearbyShipwrecks = GlobalQuadtreeHandler::getInstance().getShipwreckQuadtree()->findObjectsNearObject(playerShip, lootDistance);
	std::vector<Rock*> nearbyRocks = GlobalQuadtreeHandler::getInstance().getRockQuadtree()->findObjectsNearObject(playerShip, interactionDistance);

	if (nearestIsland == nullptr) {
		for (auto& island : nearbyIslands) {
			float distance = vm::magnitude(playerShip->getSprite().getPosition() - island->getSprite().getPosition() + island->getSprite().getGlobalBounds().getSize() / 2.f);
			if (distance <= interactionDistance) {
				nearestIsland = island;
				break;
			}
		}

		for (auto& island : islands) {
			if (std::find(nearbyIslands.begin(), nearbyIslands.end(), island.get()) == nearbyIslands.end()) {
				island->getIslandMenu()->setEnteredIsland(false);
				island->getIslandMenu()->setHasPlayerSaidNo(false);
			}
		}
	}
	// Draw the island menu for the nearest land mass
	else {
		nearestIsland->getIslandMenu()->setShip(*playerShip);
		nearestIsland->getIslandMenu()->draw();
		nearestIsland->getIslandMenu()->update();
	}

	// Handle shipwrecks
	for (auto& shipwreck : nearbyShipwrecks) {
		float distance = vm::magnitude(playerShip->getSprite().getPosition() - shipwreck->getSprite().getPosition() + shipwreck->getSprite().getGlobalBounds().getSize() / 2.f);
		if (distance <= lootDistance) {
			// Loot the shipwreck
			std::string lootDisplayString = "";
			for (auto& item : shipwreck->getLoot()) {
				lootDisplayString += "+" + std::to_string(item.amount) + " " + item.name + "\n";
				playerShip->getInventoryHandler()->addItemsToInventory(item);
				std::cout << "Looted " << item.amount << " " << item.name << " from the shipwreck" << std::endl;
			}
			lootDisplayText = sf::Text(lootDisplayString, *GlobalFontHandler::getInstance().getGlobalFont(), displayTextSize);
			displayLootText = true;
			textDisplayClock.restart();

			// Deactivate the shipwreck
			shipwreck->deactivate();

			break;
		}
	}

	// Set the nearest land mass to null if the player is no longer near it
	if (nearestIsland != nullptr) {
		sf::Vector2f shipPosition = playerShip->getSprite().getPosition(); // Ship position is already the center of the sprite
		sf::Vector2f islandPosition = nearestIsland->getSprite().getPosition() + nearestIsland->getSprite().getGlobalBounds().getSize() / 2.f; // Land mass position is the top left corner of the sprite
		float distance = vm::magnitude(shipPosition - islandPosition);

		if (distance > interactionDistance) {
			nearestIsland = nullptr;
		}
	}

	if (displayLootText) {
		sf::RenderWindow* window = GlobalValues::getInstance().getWindow();
		lootDisplayText.setPosition(playerShip->getSprite().getPosition().x, playerShip->getSprite().getPosition().y - lootDisplayText.getGlobalBounds().height + displaySpacing);
		window->draw(lootDisplayText);

		if (textDisplayClock.getElapsedTime() > textDisplayTime) {
			displayLootText = false;
		}
	}
}