#include "ScrollBar_PG.h"

using namespace PirateGame;

void ScrollBar::setUpScrollBar(sf::Vector2f scrollBarTrackOffset, float scrollBarTrackLength, sf::Vector2f interactablePositionOffset, float interactableMenuLength) {
	this->scrollBarTrackOffset = scrollBarTrackOffset;
	this->scrollBarTrackLength = scrollBarTrackLength;
	this->interactablePositionOffset = interactablePositionOffset;
	this->interactableMenuLength = interactableMenuLength;

	// Set scales
	scrollBarThumb.setScale(scale);
	scrollBarTrack.setScale(scale);
}

void ScrollBar::update(sf::Vector2f menuPosition) {
	// Set positions
	scrollBarTrack.setPosition(menuPosition + scrollBarTrackOffset);
	scrollBarThumb.setPosition(scrollBarTrack.getPosition());

	float currentY = menuPosition.y + interactablePositionOffset.y;
	for (auto& interactable : interactables) {
		interactable->setPosition(sf::Vector2f(menuPosition.x + interactablePositionOffset.x, currentY));
		currentY += interactable->getSprite().getGlobalBounds().height + spacing;
	}

	// Update the scroll bar handle's position
	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*GlobalValues::getInstance().getWindow()));

	if (scrollBarThumb.getGlobalBounds().contains(mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		isGrabbed = true;
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		isGrabbed = false;
	}

	if (isGrabbed) {
		// Calculate the new y position of the scroll bar thumb, keeping it within the bounds of the scroll bar track
		float newY = mousePosition.y - scrollBarThumb.getGlobalBounds().height / 2;
		newY = std::max(newY, scrollBarTrack.getPosition().y);
		newY = std::min(newY, scrollBarTrack.getPosition().y + scrollBarTrack.getGlobalBounds().height - scrollBarThumb.getGlobalBounds().height);

		// Set the new position of the scroll bar thumb
		scrollBarThumb.setPosition(scrollBarThumb.getPosition().x, newY);

		// Update the positions of the interactables
		float currentY = interactablePositionOffset.y;
		for (auto& interactable : interactables) {
			interactable->setPosition(sf::Vector2f(interactable->getSprite().getPosition().x, currentY));
			currentY += interactable->getSprite().getGlobalBounds().height + spacing;
		}
	}
}

void ScrollBar::draw() {
	// Draw the scroll bar and handle
	GlobalValues::getInstance().getWindow()->draw(scrollBarTrack);
	GlobalValues::getInstance().getWindow()->draw(scrollBarThumb);
}