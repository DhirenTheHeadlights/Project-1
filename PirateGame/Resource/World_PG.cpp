#include "World_PG.h"

using namespace PirateGame;

World::World(sf::RenderWindow* window_in) {
	// Set the window
	GlobalValues::getInstance().setWindow(window_in);
	window = GlobalValues::getInstance().getWindow();
	GSM = &GlobalValues::getInstance().getGSM();

	view.setUpView();

	if (window == nullptr) {
		std::cerr << "Window is nullptr immediately after setting in World\n";
	}

	// Initalize the font
	sf::Font font;
	if (!font.loadFromFile("Fonts/times_new_roman.ttf")) {
		std::cout << "Error loading font" << std::endl;
	}
	else {
		GlobalValues::getInstance().setFont(font);
	}
	
	// Set up the world
	int x = static_cast<int>(GlobalValues::getInstance().getMapSize().x);
	int y = static_cast<int>(GlobalValues::getInstance().getMapSize().y);
	int cellSize = GlobalValues::getInstance().getCellSize();
	GlobalValues::getInstance().getMap().grid(x, y, cellSize);

	// Set up the pointers
	LMHandler = std::make_unique<LandMassHandler>();
	MH = std::make_unique<MenuHandler>();

	// Set up the world
	setUpWorld();
}

void World::setUpWorld() {
	ship.setUpShip(ShipClass::Galleon);
	LMHandler->addLandMasses(GlobalValues::getInstance().getMapSize().x / 100.f, GlobalValues::getInstance().getMapSize().x / 40.f);

	// Set the ship to a rand pos
	sf::Vector2f randPos = GlobalValues::getInstance().getMap().getRandomPosition();
	ship.getMovementHandler().setPosition(randPos);

	// Set the game state to start
	GSM->changeGameState(GameState::Start);

	// Set up the menus
	MH->createMenus();
	MH->setUpMenus();

	// Set up the view
	view.setUpView();

	// Set up the hud minimap
	MH->getHUD()->getMinimap().setLandmasses(LMHandler->getLandMasses());

	// Set up the frame rate text
	frameRateText.setFont(GlobalValues::getInstance().getFont());
	frameRateText.setCharacterSize(24);
	frameRateText.setFillColor(sf::Color::White);

	// Set up the background
	background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	background.setFillColor(sf::Color(0, 158, 163));
}

void World::createWorld(sf::Event event) {
	window->clear();

	GlobalValues::getInstance().getInputHandler().update();

	// Handle the different game states
	switch (GSM->getCurrentGameState()) {
	case GameState::Start:
		// Draw the main menu
		MH->openMenu(MenuType::StartMenu);
		break;
	case GameState::OptionsMenu:
		// Draw the options menu
		MH->openMenu(MenuType::OptionsMenu);
		break;
	case GameState::LandMassInteraction:
		// Interact with the land masses
		break;
	case GameState::End:
		// End the game and close the window
		window->close();
		break;
	case GameState::GameLoop:
		// Run the game loop
		drawGameLoop();
		gameLoop();
		MH->openMenu(MenuType::HUD);
		break;
	}

	// Close if the escape key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}

	// Frame rate calculation
	sf::Time deltaTime = frameRateClock.restart();
	frameRateUpdateTime += deltaTime;
	++frameCount;

	if (frameCount >= numFramesToAverage) {
		float frameRate = frameCount / frameRateUpdateTime.asSeconds();
		frameRateText.setString("FPS: " + std::to_string(static_cast<int>(frameRate)));
		frameRateUpdateTime = sf::Time::Zero;
		frameCount = 0;
	}

	// Set the position of the frame rate text to be in the bottom left corner
	frameRateText.setPosition(view.getView().getCenter().x - window->getSize().x / 2.f, view.getView().getCenter().y + window->getSize().y / 2.f - 2 * frameRateText.getGlobalBounds().height);

	// Temporary code to close the window
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}

	window->display();
}

void World::gameLoop() {
	// Temporary code to draw a grid
	//GlobalValues::getInstance().getMap().drawGrid(*window);

	background.setPosition(view.getView().getCenter().x - window->getSize().x / 2.f, view.getView().getCenter().y - window->getSize().y / 2.f);

	LMHandler->interactWithLandmasses(ship);

	ship.update();

	view.setCenter(ship.getMovementHandler().getPosition());

	// Set the ship for the hud
	MH->getHUD()->setShip(ship);
}

void World::drawGameLoop() {
	window->draw(background);
	LMHandler->drawLandMasses();
	window->draw(frameRateText);
	ship.draw();
}

