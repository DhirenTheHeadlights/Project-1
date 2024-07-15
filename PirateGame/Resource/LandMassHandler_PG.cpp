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
		if (randNum == 0) createLandmass(LandMassType::Island, points[i]);
		else if (randNum == 1) createLandmass(LandMassType::Rock, points[i]);
		else createLandmass(LandMassType::Shipwreck, points[i]);
	}
}

void LandMassHandler::createLandmass(LandMassType type, sf::Vector2f position) {
	std::shared_ptr<LandMass> landMass = std::make_unique<LandMass>();
	landMass->createLandMass(type);
	landMass->setPosition(position);


	GlobalQuadtreeHandler::getInstance().getLandMassQuadtree()->addObject(landMass.get());
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

void LandMassHandler::interactWithLandmasses() {
	if (nearestLandMass == nullptr) {
		std::vector<LandMass*> nearbyLandMasses = GlobalQuadtreeHandler::getInstance().getLandMassQuadtree()->findObjectsNearObject(playerShip, interactionDistance);

		for (auto& landMass : nearbyLandMasses) {
			sf::Vector2f shipPosition = playerShip->getSprite().getPosition(); // Ship position is already the center of the sprite
			sf::Vector2f landMassPosition = landMass->getSprite().getPosition() + landMass->getSprite().getGlobalBounds().getSize() / 2.f; // Land mass position is the top left corner of the sprite
			float distance = vm::magnitude(shipPosition - landMassPosition);

			if (distance <= interactionDistance && landMass->getType() == LandMassType::Island) {
				// Set this as the nearest land mass
				nearestLandMass = landMass;
				break; // Stop checking for other islands
			}
			if (distance <= lootDistance && landMass->getType() == LandMassType::Shipwreck) {
				// Loot the land mass
				std::string lootDisplayString = "";
				for (auto& item : landMass->getLoot()) {
					lootDisplayString += "+" + std::to_string(item.amount) + " " + item.name + "\n";
					playerShip->getInventoryHandler()->addItemsToInventory(item);
				}
				lootDisplayText = sf::Text(lootDisplayString, *GlobalFontHandler::getInstance().getGlobalFont(), displayTextSize);
				displayLootText = true;
				textDisplayClock.restart();
				break;
			}
		}
		// Reset the 'player said no' and enteredIsland flag for all islands not nearby
		for (auto& landMass : landmasses) {
			if (landMass->getType() == LandMassType::Island && std::find(nearbyLandMasses.begin(), nearbyLandMasses.end(), landMass.get()) == nearbyLandMasses.end()) {
				landMass->getIslandMenu()->setEnteredIsland(false);
				landMass->getIslandMenu()->setHasPlayerSaidNo(false);
			}
		}
	}
	// Draw the island menu for the nearest land mass
	else {
		nearestLandMass->getIslandMenu()->setShip(*playerShip);
		nearestLandMass->getIslandMenu()->draw();
		nearestLandMass->getIslandMenu()->update();
	}

	// Set the nearest land mass to null if the player is no longer near it
	if (nearestLandMass != nullptr) {
		sf::Vector2f shipPosition = playerShip->getSprite().getPosition(); // Ship position is already the center of the sprite
		sf::Vector2f landMassPosition = nearestLandMass->getSprite().getPosition() + nearestLandMass->getSprite().getGlobalBounds().getSize() / 2.f; // Land mass position is the top left corner of the sprite
		float distance = vm::magnitude(shipPosition - landMassPosition);

		if (distance > interactionDistance) {
			nearestLandMass = nullptr;
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