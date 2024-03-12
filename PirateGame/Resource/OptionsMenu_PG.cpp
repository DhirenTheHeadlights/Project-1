#include "OptionsMenu_PG.h"

using namespace PirateGame;

// Set up the options menu
void OptionsMenu::setUpMenu() {
	// Set up the menu sprite
	menu.setTexture(GlobalTextureHandler::getInstance().getOptionsMenuTextures().getBackground());
	menu.setPosition(static_cast<float>(window->getSize().x / 2u) - menu.getGlobalBounds().getSize().x / 2, 
				     static_cast<float>(window->getSize().y / 2u) - menu.getGlobalBounds().getSize().y / 2);

	// Set up the title text
	titleText = sf::Text("Options", font, textSize);
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition(menu.getPosition().x + menu.getGlobalBounds().getSize().x / 2 - titleText.getGlobalBounds().getSize().x / 2, 
						  menu.getPosition().y - titleText.getGlobalBounds().getSize().y - padding);

	// Add the interactables to the menu and set their positions
	addInteractablesToMenu();
	setInteractablePositions();
}

void OptionsMenu::setInteractablePositions() {
	// Set the positions of the interactables to be centered under the menu
	sf::Vector2f position = sf::Vector2f(menu.getPosition().x + menu.getGlobalBounds().getSize().x / 2, 
									     menu.getPosition().y + menu.getGlobalBounds().getSize().y + padding);
	interactables[0]->setPosition(position); // Only the back button is added to the menu

	// Set the positions of the tabs
	sf::Vector2f tabPosition = sf::Vector2f(menu.getPosition().x + tabButtonX, menu.getPosition().y + tabButtonY);
	for (auto& tab : tabButtons) {
		tab->setPosition(tabPosition + tab->getSprite().getGlobalBounds().getSize());
		tabPosition.x += tab->getSprite().getGlobalBounds().getSize().x;
	}

	setTabInteractablePositions(generalTabInteractables);
	setTabInteractablePositions(graphicsTabInteractables);
	setTabInteractablePositions(audioTabInteractables);
	setTabInteractablePositions(controlsTabInteractables);
}

void OptionsMenu::setTabInteractablePositions(std::vector<std::unique_ptr<Interactable>>& tabInteractables) {
	// Set the positions of the interactables to be under the menu
	sf::Vector2f leftPosition = sf::Vector2f(menu.getPosition().x + leftColumnX, menu.getPosition().y + leftColumnY);
	sf::Vector2f rightPosition = sf::Vector2f(menu.getPosition().x + rightColumnX, menu.getPosition().y + rightColumnY);

	for (auto& interactable : tabInteractables) {
		interactable->setPosition(rightPosition);
		interactable->getText().setPosition(leftPosition);
		float offset = interactable->getSprite().getGlobalBounds().getSize().y + rowSpacing;
		leftPosition.y += offset;
		rightPosition.y += offset;
	}
}

/// Tabs

void OptionsMenu::addInteractablesToMenu() {
	// Grab the global game state manager
	GSM = &GlobalValues::getInstance().getGSM();

	// Add the tabs to the menu
	addTabInteractable([this]() { currentTab = Tab::General; }, "General");
	addTabInteractable([this]() { currentTab = Tab::Graphics; }, "Graphics");
	addTabInteractable([this]() { currentTab = Tab::Audio; }, "Audio");
	addTabInteractable([this]() { currentTab = Tab::Controls; }, "Controls");


	addGeneralTabInteractables();
	addGraphicsTabInteractables();
	addAudioTabInteractables();
	addControlsTabInteractables();

	// Create the back button
	std::function<void()> backFunc = [this]() { 
		if (GSM->hasGameStarted()) { 
			GSM->changeGameState(GameState::GameLoop); 
		} // Enter the game loop again if the game has started
		else { 
			GSM->changeGameState(GameState::Start);
		}
	};
	std::unique_ptr<Button> backButton = std::make_unique<Button>(backFunc);
	sf::Text backText = sf::Text("Back", font, interactableTextSize);
	backButton->createInteractable(GlobalTextureHandler::getInstance().getOptionsMenuTextures().getLeftInteractable(), backText);
	addInteractableToMenu(std::move(backButton));
}

void OptionsMenu::addTabInteractable(std::function<void()> func, std::string name) {
	std::unique_ptr<Button> tabButton = std::make_unique<Button>(func);
	sf::Text text = sf::Text(name, font, interactableTextSize);
	tabButton->createInteractable(GlobalTextureHandler::getInstance().getOptionsMenuTextures().getTab(), text);
	tabButtons.push_back(std::move(tabButton));
}

/// Set up the interactables for the tabs

void OptionsMenu::addGeneralTabInteractables() {
	// Add the interactables to the general tab
	std::vector<std::pair<std::function<void()>, std::string>> screenPair;

	screenPair.push_back(std::make_pair(std::function<void()>([]() {
		// Set the game to fullscreen
		GlobalValues::getInstance().getWindow()->create(sf::VideoMode::getDesktopMode(), "Pirate Game", sf::Style::Fullscreen);
		}), "Fullscreen"));

	screenPair.push_back(std::make_pair(std::function<void()>([]() {
		// Set the game to windowed
		GlobalValues::getInstance().getWindow()->create(sf::VideoMode(1920, 1080), "Pirate Game", sf::Style::Default);
		}), "Windowed"));

	addDropDownInteractable(screenPair, "Screen", generalTabInteractables);

	std::vector<std::pair<std::function<void()>, std::string>> resolutionPair;

	resolutionPair.push_back(std::make_pair(std::function<void()>([]() {
		GlobalValues::getInstance().getWindow()->setSize(sf::Vector2u(1920, 1080));
		}), "1920x1080"));
	resolutionPair.push_back(std::make_pair(std::function<void()>([]() {
		GlobalValues::getInstance().getWindow()->setSize(sf::Vector2u(1600, 900));
		}), "1600x900"));
	resolutionPair.push_back(std::make_pair(std::function<void()>([]() {
		GlobalValues::getInstance().getWindow()->setSize(sf::Vector2u(1280, 720));
		}), "1280x720"));
	resolutionPair.push_back(std::make_pair(std::function<void()>([]() {
		GlobalValues::getInstance().getWindow()->setSize(sf::Vector2u(640, 480));
		}), "640x480"));

	addDropDownInteractable(resolutionPair, "Resolution", generalTabInteractables);
}

void OptionsMenu::addGraphicsTabInteractables() {
	// Add the interactables to the graphics tab
	std::function<void(float value)> brightnessSliderFunc = [this](float value) { GSM->changeGameState(GameState::Start); };
	addSliderInteractable(brightnessSliderFunc, "Brightness", graphicsTabInteractables);

	std::function<void(float value)> contrastSliderFunc = [this](float value) { GSM->changeGameState(GameState::Start); };
	addSliderInteractable(contrastSliderFunc, "Contrast", graphicsTabInteractables);

	std::vector<std::pair<std::function<void()>, std::string>> qualityPair;
	qualityPair.push_back(std::make_pair(std::function<void()>([]() {}), "Low"));
	qualityPair.push_back(std::make_pair(std::function<void()>([]() {}), "Medium"));
	qualityPair.push_back(std::make_pair(std::function<void()>([]() {}), "High"));
	addDropDownInteractable(qualityPair, "Quality", graphicsTabInteractables);
}

void OptionsMenu::addAudioTabInteractables() {
	// Add the interactables to the audio tab
	std::function<void(float value)> musicSliderFunc = [this](float value) { GSM->changeGameState(GameState::Start); };
	addSliderInteractable(musicSliderFunc, "Music Volume", audioTabInteractables);

	std::function<void(float value)> sfxSliderFunc = [this](float value) { GlobalValues::getInstance().setGlobalVolume(value); };
	addSliderInteractable(sfxSliderFunc, "SFX Volume", audioTabInteractables);

	std::function<void(float value)> voiceSliderFunc = [this](float value) { GSM->changeGameState(GameState::Start); };
	addSliderInteractable(voiceSliderFunc, "Voice Volume", audioTabInteractables);
}

void OptionsMenu::addControlsTabInteractables() {
	// Add the interactables to the controls tab
	std::function<void()> rebindFunc = [this]() { GSM->changeGameState(GameState::Start); };
	addButtonInteractable(rebindFunc, "Rebind", controlsTabInteractables);
}

/// Helpers

void OptionsMenu::addDropDownInteractable(std::vector<std::pair<std::function<void()>, std::string>> options, std::string name, std::vector<std::unique_ptr<Interactable>>& tabInteractables) {
	std::unique_ptr<DropDown> dropDown = std::make_unique<DropDown>(options);
	sf::Text text = sf::Text(name, font, interactableTextSize);
	dropDown->createInteractable(GlobalTextureHandler::getInstance().getOptionsMenuTextures().getDropDown(), text);
	dropDown->setOptionsBoxSprite(GlobalTextureHandler::getInstance().getOptionsMenuTextures().getRightInteractable());
	tabInteractables.push_back(std::move(dropDown));
}

void OptionsMenu::addSliderInteractable(std::function<void(float value)> func, std::string name, std::vector<std::unique_ptr<Interactable>>& tabInteractables) {
	std::unique_ptr<Slider> slider = std::make_unique<Slider>(func);
	sf::Text text = sf::Text(name, font, interactableTextSize);
	slider->createInteractable(GlobalTextureHandler::getInstance().getOptionsMenuTextures().getRightInteractable(), text);
	tabInteractables.push_back(std::move(slider));
}

void OptionsMenu::addButtonInteractable(std::function<void()> func, std::string name, std::vector<std::unique_ptr<Interactable>>& tabInteractables) {
	std::unique_ptr<Button> button = std::make_unique<Button>(func);
	sf::Text text = sf::Text(name, font, interactableTextSize);
	button->createInteractable(GlobalTextureHandler::getInstance().getOptionsMenuTextures().getRightInteractable(), text);
	tabInteractables.push_back(std::move(button));
}

/// Draw/Interact with the menu

void OptionsMenu::interactWithMenuItems() {
	// Interact with the tabs
	for (auto& tab : tabButtons) {
		tab->interact();
	}

	// Interact with the interactables for the current tab
	switch (currentTab) {
	case Tab::General:
		for (auto& interactable : generalTabInteractables) {
			interactable->interact();
		}
		break;
	case Tab::Graphics:
		for (auto& interactable : graphicsTabInteractables) {
			interactable->interact();
		}
		break;
	case Tab::Audio:
		for (auto& interactable : audioTabInteractables) {
			interactable->interact();
		}
		break;
	case Tab::Controls:
		for (auto& interactable : controlsTabInteractables) {
			interactable->interact();
		}
		break;
	}

	// Interact with the menu items
	for (auto& interactable : interactables) {
		interactable->interact();
	}
}

// Draw the menu
void OptionsMenu::draw() {
	// Draw the menu background
	window->draw(menu);
	window->draw(titleText);

	// Draw the interactables
	for (auto& interactable : interactables) {
		interactable->setTextColor(sf::Color::White);
		interactable->draw();
	}

	// Draw the tabs
	for (auto& tab : tabButtons) {
		// Set the color of the tabs
		tab->draw();
	}

	// Draw the interactables for the current tab
	switch (currentTab) {
	case Tab::General:
		drawTabInteractables(generalTabInteractables);
		break;
	case Tab::Graphics:
		drawTabInteractables(graphicsTabInteractables);
		break;
	case Tab::Audio:
		drawTabInteractables(audioTabInteractables);
		break;
	case Tab::Controls:
		drawTabInteractables(controlsTabInteractables);
		break;
	}

	// Interact with the menu items
	interactWithMenuItems();
}

// General function to draw the tab interactables
void OptionsMenu::drawTabInteractables(std::vector<std::unique_ptr<Interactable>>& tabInteractables) {
	// Draw the interactables in reverse order so that the top interactable is draw last
	// This means that the drop down menu will be drawn on top of the other interactables and will
	// be visible
	for (auto it = tabInteractables.rbegin(); it != tabInteractables.rend(); ++it) {
		(*it)->getText().setFillColor(sf::Color::Black);
		(*it)->draw();
	}
}
