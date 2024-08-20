#include "InventoryMenu_PG.h"

using namespace PirateGame;

void InventoryMenu::setUpMenu() {
	menu.setTexture(context.GTH->getInventoryTextures().getBackground());

	interactableTextSize = 25;

	// Create Pop-up frame and set scales
	scale = sf::Vector2f(xProportion * window->getSize().x / menu.getGlobalBounds().getSize().x, yProportion * window->getSize().y / menu.getGlobalBounds().getSize().y);
	menu.setScale(scale);
	shipDisplayBackground.setScale(scale);
	inventoryDisplayBackground.setScale(scale);

	shipDisplayPosition = sf::Vector2f(shipDisplayPosition.x * scale.x, shipDisplayPosition.y * scale.y);
	shipInfoPosition = sf::Vector2f(shipInfoPosition.x * scale.x, shipInfoPosition.y * scale.y);
	inventoryPosition = sf::Vector2f(inventoryPosition.x * scale.x, inventoryPosition.y * scale.y);
	scrollBarPosition = sf::Vector2f(scrollBarPosition.x * scale.x, scrollBarPosition.y * scale.y);

	// Set up the title text
	titleText = sf::Text("Inventory", font, textSize);
	titleText.setFillColor(sf::Color::White);

	// Prepare the ship display
	shipDisplaySprite.setTexture(context.GTH->getShipTextures().getShipTextureManagerByRegion(ship->getBirthRegion()).getTexture(ship->getShipClass()));
	shipDisplayBackground.setTexture(context.GTH->getInventoryTextures().getInventoryShipDisplay());

	// Prepare the inventory display background
	inventoryDisplayBackground.setTexture(context.GTH->getInventoryTextures().getInventoryDisplay());

	// Set up the scroll bar
	scrollBar.setUpScrollBar(scrollBarPosition, inventoryDisplayBackground.getGlobalBounds().getSize().y, inventoryPosition, inventoryDisplayBackground.getGlobalBounds().getSize(), scale);

	// Add the interactables to the menu and set their positions
	addInteractablesToMenu();
}

void InventoryMenu::addInteractablesToMenu() {
	inventoryBoxes.clear();
	this->inventory = ship->getInventoryHandler()->getInventory();
	for (auto& item : inventory){
		std::shared_ptr<TextDisplayBox> textDisplayBox = std::make_shared<TextDisplayBox>();
		sf::Text text = sf::Text(item.name + ": " + std::to_string(item.amount), font, interactableTextSize);
		textDisplayBox->createInteractable(context.GTH->getInventoryTextures().getInventoryItemDisplay(), text, scale);
		textDisplayBox->getText().setFillColor(sf::Color::Black);
		inventoryBoxes.push_back(textDisplayBox);
	}

	shipDisplayInfo.clear();
	addTextDisplayBox(sf::Text(
		ship->getShipClassString() + "\n" +
		"Level: " + std::to_string(ship->getplayerLevel()) + "\n" +
		"Health: " + floatToString(ship->getHealth()) + " / " + floatToString(ship->getSpecificShipProperties().maxHealth) + "\n"
		"Speed: " + floatToString(ship->getSpecificShipProperties().baseSpeed) + "\n"
		"Cannons: " + std::to_string(ship->getSpecificShipProperties().numCannons) + "\n", 
		font, interactableTextSize), context.GTH->getInventoryTextures().getInventoryTextDisplay(), shipDisplayInfo, scale);
	shipDisplayInfo[0].getText().setFillColor(sf::Color::Black);
}

void InventoryMenu::setInteractablePositions() {
	// Set the position of the menu and title text
	menu.setPosition(window->getView().getCenter() - sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
	titleText.setPosition(menu.getPosition().x + menu.getGlobalBounds().getSize().x / 2 - titleText.getGlobalBounds().getSize().x / 2,
		menu.getPosition().y - titleText.getGlobalBounds().getSize().y - padding);

	// Set the position of the ship display
	shipDisplayBackground.setPosition(menu.getPosition() + shipDisplayPosition);
	shipDisplaySprite.setPosition(shipDisplayBackground.getPosition() + sf::Vector2f(0.5f * shipDisplayBackground.getGlobalBounds().getSize().x - 0.5f * shipDisplaySprite.getGlobalBounds().getSize().x,
		0.5f * shipDisplayBackground.getGlobalBounds().getSize().y - 0.5f * shipDisplaySprite.getGlobalBounds().getSize().y));

	// Set the position of the ship display info
	shipDisplayInfo[0].setPosition(menu.getPosition() + shipInfoPosition);

	// Set the position of the inventory display
	inventoryDisplayBackground.setPosition(menu.getPosition() + inventoryPosition);
}

void InventoryMenu::interactWithMenuItems() {
	/* nothing here yet */
}

void InventoryMenu::update() {
	if (inventory != ship->getInventoryHandler()->getInventory()) {
		addInteractablesToMenu();
	}
	setInteractablePositions();
	interactWithMenuItems();
	scrollBar.update(menu.getPosition(), inventoryBoxes);

	// Update ship display info
	shipDisplayInfo[0].getText().setString(
		ship->getShipClassString() + "\n" +
		"Level: " + std::to_string(ship->getplayerLevel()) + "\n" +
		"Health: " + floatToString(ship->getHealth()) + " / " + floatToString(ship->getSpecificShipProperties().maxHealth) + "\n"
		"Speed: " + floatToString(ship->getSpecificShipProperties().baseSpeed) + "\n"
		"Cannons: " + std::to_string(ship->getSpecificShipProperties().numCannons) + "\n");
}

void InventoryMenu::draw() {
    window->draw(menu);
    window->draw(titleText);
    window->draw(shipDisplayBackground);
    window->draw(shipDisplaySprite);

    for (auto& box : shipDisplayInfo) {
        box.draw(window);
    }

    // Flag so that the first box size is only set once
    bool hasSetFirstBoxSize = false;

    // Get the scroll percentage from the scroll bar
    float scrollPercentage = scrollBar.getScrollPercentage();

    for (auto& box : inventoryBoxes) {
        auto& sprite = box->getSprite();
        auto& text = box->getText();
        float boxTop = sprite.getPosition().y;
        float boxBottom = boxTop + sprite.getGlobalBounds().height;
        float displayTop = menu.getPosition().y + inventoryPosition.y;
        float displayBottom = displayTop + inventoryDisplayBackground.getGlobalBounds().height;

        // Calculate the visible portion of the box based on the original position and size
        float visibleTop = std::max(boxTop, displayTop);
        float visibleBottom = std::min(boxBottom, displayBottom);
        float visibleHeight = visibleBottom - visibleTop;

        // Check if the box is within or partially within the display area
        if (visibleHeight > 0) {
            sf::IntRect originalRect = sprite.getTextureRect();
            sf::IntRect visibleRect = originalRect;
            float textureOffsetY = visibleTop - boxTop;

            // Adjust the texture rect for the visible portion
            visibleRect.top += static_cast<int>(textureOffsetY);
            visibleRect.height = static_cast<int>(visibleHeight);

            // Set the modified texture rect without altering the sprite's position
            sprite.setTextureRect(visibleRect);

            // Apply an offset to the draw position instead of changing the sprite's position
            sf::RenderStates states;
            states.transform.translate(0, textureOffsetY);

            // Draw the sprite with the offset transform applied
            window->draw(sprite, states);

            // Restore the original texture rect
            sprite.setTextureRect(originalRect);

            // Render the text onto a render texture to clip it
            sf::RenderTexture renderTexture;
            sf::FloatRect textBounds = text.getGlobalBounds();

            // Create the render texture with enough size to hold the full text
            if (!renderTexture.create(static_cast<unsigned int>(textBounds.width), static_cast<unsigned int>(textBounds.height))) {
                std::cout << "Failed to create render texture for interactable: " + text.getString().toAnsiString() << std::endl;
                continue;  // Skip rendering if texture creation fails
            }

            renderTexture.clear(sf::Color::Transparent);
            renderTexture.draw(text);  // Render the full text to the render texture
            renderTexture.display();

            sf::Sprite textSprite(renderTexture.getTexture());

            // Set the clipping area for the text
            float textTop = text.getPosition().y;
            float textBottom = textTop + textBounds.height;
            float textVisibleTop = std::max(textTop, visibleTop);
            float textVisibleBottom = std::min(textBottom, visibleBottom);
            float textVisibleHeight = textVisibleBottom - textVisibleTop;

            if (textVisibleHeight > 0) {
                sf::IntRect textVisibleRect(0, static_cast<int>(textVisibleTop - textTop), static_cast<int>(textBounds.width), static_cast<int>(textVisibleHeight));
                textSprite.setTextureRect(textVisibleRect);

                // Position the sprite correctly
                textSprite.setPosition(text.getPosition().x, textVisibleTop);
                
                sf::RectangleShape clipRect(sf::Vector2f(visibleRect.width, visibleRect.height));
                clipRect.setPosition(text.getPosition().x, visibleTop);
                clipRect.setFillColor(sf::Color::Transparent);
                clipRect.setOutlineColor(sf::Color::Red);
                clipRect.setOutlineThickness(1.f);
                window->draw(clipRect);

                // Draw the clipped text sprite
                window->draw(textSprite);
            }

            // Set the first box size for the scrollbar only once, and only if the scroll percentage has changed
            if (!hasSetFirstBoxSize && std::fabs(scrollPercentage - scrollBar.getScrollPercentage()) > 0.0001f) {
                scrollBar.setFirstBoxSize(sf::Vector2f(visibleRect.width, visibleRect.height));
                hasSetFirstBoxSize = true;
            }
        }
    }

    scrollBar.draw();
}

