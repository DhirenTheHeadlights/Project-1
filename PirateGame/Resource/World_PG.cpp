#include "World_PG.h"

using namespace PirateGame;

void World::setUpWorld() {
	// Set the window
	GlobalValues::getInstance().setWindow(window);
	view.setUpView();

	// Set up the map
	GlobalMap::getInstance().initializeMap();

	// Set up the player ship
	playerShip = std::make_unique<PlayerShip>();

	// Set up the player ship
	playerShip->setUpShip(ShipClass::ManOWar);
	playerShip->getCannonHandler()->setCannonballHashmap(GlobalHashmapHandler::getInstance().getCannonballHashmap());

	// Set up the land masses
	LMHandler.addLandMasses(numLandMasses, distanceBetweenLandMasses);

	// Set up the enemy ships
	ESH.setLandmasses(LMHandler.getLandMasses());
	ESH.addEnemyShips(numEnemyShips);
	ESH.setPlayerShip(playerShip.get());

	// Set up the collision manager
	CM.setPlayerShip(playerShip.get());
	CM.setEnemyShips(ESH.getEnemyShips());
	CM.setLandMasses(LMHandler.getLandMasses());

	// Set the game state to start
	GlobalGameStateManager::getInstance().changeGameState(GameState::Start);

	// Set up the menus
	MH.createMenus();
	MH.setUpMenus();

	// Set up the view
	view.setUpView();

	// Set up the hud minimap
	MH.getHUD()->getMinimap().setLandmasses(LMHandler.getLandMasses());
	MH.getHUD()->getMinimap().setEnemyShips(ESH.getEnemyShips());
	MH.getHUD()->setPlayerShip(*playerShip.get());

	// Set up the frame rate text
	frameRateText.setFont(*GlobalFontHandler::getInstance().getGlobalFont());
	frameRateText.setCharacterSize(24u);
	frameRateText.setFillColor(sf::Color::White);

	// Set up the experience text
	experience.setFont(*GlobalFontHandler::getInstance().getGlobalFont());
	experience.setCharacterSize(24u);
	experience.setFillColor(sf::Color::White);

	// Set up the background
	background.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	background.setFillColor(backgroundColor);
}

void World::drawGameLoop() {
	window->draw(background);
	LMHandler.drawLandMasses();
	window->draw(frameRateText);
	window->draw(experience);
	playerShip->draw();
	ESH.draw();
}

