#include "World_PG.h"

using namespace PirateGame;

World::World(sf::RenderWindow* window_in) {
	// Set the window
	GlobalValues::getInstance().setWindow(window_in);
	window = GlobalValues::getInstance().getWindow();

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
	MH = std::make_unique<MenuHandler>(GSM);

	// Set up the world
	setUpWorld();
}

void World::setUpWorld() {
	ship.createShip(ShipType::Player, ShipClass::Frigate);
	LMHandler->addLandMasses(100, 500.f);

	// Set the game state to start
	GSM.changeGameState(GameState::Start);

	// Set up the menus
	MH->createMenus();
	MH->setUpMenus();

	// Set up the view
	view.setUpView();
}

void World::createWorld(sf::Event event) {
	window->clear();
	GlobalValues::getInstance().getInputHandler().update();

	// Handle the different game states
	switch (GSM.getCurrentGameState()) {
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
		gameLoop();
		MH->openMenu(MenuType::HUD);
		break;
	}

	// Close if the escape key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}

	float frameRate = 1.f / frameRateClock.restart().asSeconds();
	sf::Text frameRateText;
	frameRateText.setFont(GlobalValues::getInstance().getFont());
	frameRateText.setString(std::to_string(static_cast<int>(frameRate)));
	frameRateText.setCharacterSize(24);
	frameRateText.setFillColor(sf::Color::White);
	frameRateText.setPosition(0, 0);

	window->display();
}

void World::gameLoop() {
	// Temp background rect for water
	sf::RectangleShape background(sf::Vector2f(10000, 10000));
	background.setFillColor(sf::Color(0, 158, 163));
	window->draw(background);

	// Temporary code to draw a grid
	//map.drawGrid(window);

	// Draw the land masses
	LMHandler->drawLandMasses(ship);

	// Draw the ship
	ship.updateAndDraw();
	view.setCenter(ship.getMovementHandler().getPosition());

	// Set the ship for the hud
	//MH->getHUD();

	// Temporary code to close the window
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}
}

