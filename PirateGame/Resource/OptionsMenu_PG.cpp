#include "OptionsMenu_PG.h"

using namespace PirateGame;

// Set up the options menu
void OptionsMenu::setUpMenu() {
	// Set up the background rectangle
	backgroundRect = sf::RectangleShape(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
	backgroundRect.setFillColor(backgroundColor);

	// Set the center of the background rectangle
	sf::Vector2f center = sf::Vector2f(backgroundRect.getSize().x / 2, backgroundRect.getSize().y / 2);

	// Set the title
	titleText = sf::Text("Options", font, static_cast<unsigned int>(100.f));
	titleText.setFillColor(textColor);
	titleText.setPosition(sf::Vector2f(center.x - 160.f, 100.f));

	// Set the size of the menu
	size = sf::Vector2f(1000, 500);

	// Set up the menu rectangle
	menu.setSize(size);
	menu.setFillColor(textColor);
	menu.setPosition(sf::Vector2f(center.x - menu.getSize().x / 2, center.y - menu.getSize().y / 2));

	// Set up the rectangle housing all of the tabs
	tabBar.setSize(sf::Vector2f(menu.getSize().x, 50));
	tabBar.setFillColor(sf::Color(0, 0, 0, 200));
	tabBar.setPosition(menu.getPosition());

	// Add the interactables to the menu and set their positions
	addInteractables();
	setInteractablePositions();
}

// Set the position of the interactables
void OptionsMenu::setInteractablePositions() {
	// Set the positions of the interactables to be in the center of the screen, slightly below the title
	sf::Vector2f position = sf::Vector2f(backgroundRect.getSize().x / 2 - interactableSize.x / 2, backgroundRect.getSize().y / 2 - interactableSize.y / 2 + 100);
	for (auto& interactable : interactables) {
		interactable->setPosition(sf::Vector2f(menu.getPosition().x, menu.getPosition().y + menu.getSize().y + 1.f));
		position.y += interactableSize.y + 10;
	}

	// Set the positions of the tabs
	sf::Vector2f tabPosition = sf::Vector2f(menu.getPosition().x, menu.getPosition().y);
	for (auto& tab : tabButtons) {
		tab->setPosition(tabPosition);
		tabPosition.x += tabSize.x;
	}

	setTabInteractablePositions(generalTabInteractables);
	setTabInteractablePositions(videoTabInteractables);
	setTabInteractablePositions(audioTabInteractables);
	setTabInteractablePositions(controlsTabInteractables);
}

// General function to set the position of the tab interactables
void OptionsMenu::setTabInteractablePositions(std::vector<std::unique_ptr<Interactable>>& tabInteractables) {
	// Set the positions of the interactables for the general tab
	sf::Vector2f position = sf::Vector2f(tabBar.getPosition().x, tabBar.getPosition().y + tabBar.getSize().y);
	for (auto& interactable : tabInteractables) {
		interactable->setPosition(sf::Vector2f(menu.getPosition().x + menu.getSize().x * 0.25f, position.y));
		interactable->setTextPosition(position);
		position.y += interactableSize.y + 1.f;
	}
}

// Add the interactables to the menu
void OptionsMenu::addInteractables() {
	// Add the tabs to the menu
	std::function<void()> generalTabFunc = [this]() { currentTab = Tab::General; };
	std::unique_ptr<Button> generalTabButton = std::make_unique<Button>(generalTabFunc, font);
	generalTabButton->setUpInteractable(tabSize);
	generalTabButton->setString("General");
	tabButtons.push_back(std::move(generalTabButton));

	addGeneralTabInteractables();

	std::function<void()> graphicsTabFunc = [this]() { currentTab = Tab::Graphics; };
	std::unique_ptr<Button> graphicsTabButton = std::make_unique<Button>(graphicsTabFunc, font);
	graphicsTabButton->setUpInteractable(tabSize);
	graphicsTabButton->setString("Graphics");
	tabButtons.push_back(std::move(graphicsTabButton));

	addGraphicsTabInteractables();

	std::function<void()> audioTabFunc = [this]() { currentTab = Tab::Audio; };
	std::unique_ptr<Button> audioTabButton = std::make_unique<Button>(audioTabFunc, font);
	audioTabButton->setUpInteractable(tabSize);
	audioTabButton->setString("Audio");
	tabButtons.push_back(std::move(audioTabButton));

	addAudioTabInteractables();

	std::function<void()> controlsTabFunc = [this]() { currentTab = Tab::Controls; };
	std::unique_ptr<Button> controlsTabButton = std::make_unique<Button>(controlsTabFunc, font);
	controlsTabButton->setUpInteractable(tabSize);
	controlsTabButton->setString("Controls");
	tabButtons.push_back(std::move(controlsTabButton));

	addControlsTabInteractables();

	// Create the back button
	std::function<void()> backFunc = [this]() { GSM.changeGameState(GameState::Start); };
	std::unique_ptr<Button> backButton = std::make_unique<Button>(backFunc, font);
	backButton->setUpInteractable(sf::Vector2f(size.x, 50.f));
	backButton->setString("Back");
	addInteractableToMenu(std::move(backButton));
}

void OptionsMenu::addGeneralTabInteractables() {
	// Add the interactables to the general tab
	std::function<void()> fullscreenFunc = [this]() { GSM.changeGameState(GameState::Start); };
	std::unique_ptr<Button> fullscreenButton = std::make_unique<Button>(fullscreenFunc, font);
	fullscreenButton->setUpInteractable(sf::Vector2f(0.75f * size.x, 50.f));
	fullscreenButton->setString("Fullscreen");
	generalTabInteractables.push_back(std::move(fullscreenButton));

	std::function<void()> resolutionFunc = [this]() { GSM.changeGameState(GameState::Start); };
	std::unique_ptr<Button> resolutionButton = std::make_unique<Button>(resolutionFunc, font);
	resolutionButton->setUpInteractable(sf::Vector2f(0.75f * size.x, 50.f));
	resolutionButton->setString("Resolution");
	generalTabInteractables.push_back(std::move(resolutionButton));
}

void OptionsMenu::addGraphicsTabInteractables() {
	// Add the interactables to the graphics tab
	std::function<void()> vsyncFunc = [this]() { GSM.changeGameState(GameState::Start); };
	std::unique_ptr<Button> vsyncButton = std::make_unique<Button>(vsyncFunc, font);
	vsyncButton->setUpInteractable(sf::Vector2f(0.75f * size.x, 50.f));
	vsyncButton->setString("Vsync");
	videoTabInteractables.push_back(std::move(vsyncButton));
}


void OptionsMenu::addAudioTabInteractables() {
	// Add the interactables to the audio tab
	std::function<void(float value)> musicSliderFunc = [this](float value) { GSM.changeGameState(GameState::Start); };
	std::unique_ptr<Slider> musicSlider = std::make_unique<Slider>(musicSliderFunc, font);
	musicSlider->setUpInteractable(sf::Vector2f(0.75f * size.x, 50.f));
	musicSlider->setString("Music Volume");
	audioTabInteractables.push_back(std::move(musicSlider));

	std::function<void(float value)> sfxSliderFunc = [this](float value) { GSM.changeGameState(GameState::Start); };
	std::unique_ptr<Slider> sfxSlider = std::make_unique<Slider>(sfxSliderFunc, font);
	sfxSlider->setUpInteractable(sf::Vector2f(0.75f * size.x, 50.f));
	sfxSlider->setString("SFX Volume");
	audioTabInteractables.push_back(std::move(sfxSlider));

	std::function<void(float value)> voiceSliderFunc = [this](float value) { GSM.changeGameState(GameState::Start); };
	std::unique_ptr<Slider> voiceSlider = std::make_unique<Slider>(voiceSliderFunc, font);
	voiceSlider->setUpInteractable(sf::Vector2f(0.75f * size.x, 50.f));
	voiceSlider->setString("Voice Volume");
	audioTabInteractables.push_back(std::move(voiceSlider));
}

void OptionsMenu::addControlsTabInteractables() {
	// Add the interactables to the controls tab
	std::function<void()> rebindFunc = [this]() { GSM.changeGameState(GameState::Start); };
	std::unique_ptr<Button> rebindButton = std::make_unique<Button>(rebindFunc, font);
	rebindButton->setUpInteractable(sf::Vector2f(0.75f * size.x, 50.f));
	rebindButton->setString("Rebind Controls");
	controlsTabInteractables.push_back(std::move(rebindButton));
}

// Interact with the menu items
void OptionsMenu::interactWithMenuItems() {
	// Interact with the tabs
	for (auto& tab : tabButtons) {
		tab->interact(window);
	}

	// Interact with the interactables for the current tab
	switch (currentTab) {
	case Tab::General:
		for (auto& interactable : generalTabInteractables) {
			interactable->interact(window);
		}
		break;
	case Tab::Graphics:
		for (auto& interactable : videoTabInteractables) {
			interactable->interact(window);
		}
		break;
	case Tab::Audio:
		for (auto& interactable : audioTabInteractables) {
			interactable->interact(window);
		}
		break;
	case Tab::Controls:
		for (auto& interactable : controlsTabInteractables) {
			interactable->interact(window);
		}
		break;
	}
}

// Draw the menu
void OptionsMenu::draw(sf::RenderWindow& window) {
	// Draw the menu background
	window.draw(backgroundRect);
	window.draw(menu);
	window.draw(tabBar);
	window.draw(titleText);

	// Draw the interactables
	for (auto& interactable : interactables) {
		interactable->setTextColor(sf::Color::White);
		interactable->setOutlineThickness(0.f);
		interactable->updateColors();
		interactable->draw(window);
	}

	// Draw the tabs
	for (auto& tab : tabButtons) {
		// Set the color of the tabs
		tab->setBackgroundColor(sf::Color::Transparent);
		tab->setForegroundColor(textColor);
		tab->setFrameColor(sf::Color::Transparent);
		tab->setTextColor(sf::Color::White);
		tab->updateColors();
		tab->draw(window);
	}

	// Draw the interactables for the current tab
	switch (currentTab) {
	case Tab::General:
		drawTabInteractables(generalTabInteractables);
		break;
	case Tab::Graphics:
		drawTabInteractables(videoTabInteractables);
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
	for (auto& interactable : tabInteractables) {
		interactable->setTextColor(sf::Color::White);
		interactable->setOutlineThickness(0.f);
		interactable->updateColors();
		interactable->draw(window);
	}
}
