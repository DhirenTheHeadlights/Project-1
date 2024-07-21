#include "ScrollBar_PG.h"

using namespace PirateGame;

void ScrollBar::setUpScrollBar(sf::Vector2f scrollBarTrackOffset, float scrollBarTrackLength, sf::Vector2f interactablePositionOffset, sf::Vector2f size, sf::Vector2f scale) {
	this->scrollBarTrackOffset = scrollBarTrackOffset;
	this->scrollBarTrackLength = scrollBarTrackLength;
	this->interactablePositionOffset = interactablePositionOffset;
	this->interactableMenuLength = vertical ? size.y : size.x;

	// Set scales
	scrollBarThumb.setScale(scale);
	scrollBarTrack.setScale(scale);

	// Set up the view
	menuView.setSize(size);
}

void ScrollBar::updateInteractablePositions(sf::Vector2f menuPosition) {
	float currentY = menuPosition.y + interactablePositionOffset.y;
	for (auto& interactable : interactables) {
		interactable->setPosition(sf::Vector2f(menuPosition.x + interactablePositionOffset.x, currentY));
		currentY += interactable->getSprite().getGlobalBounds().height + spacing;
	}
}

void ScrollBar::updateScrollBarPositions(sf::Vector2f menuPosition) {
	menuView.setCenter(menuPosition + interactablePositionOffset + menuView.getSize() / 2.f);

	scrollBarTrack.setPosition(menuPosition + scrollBarTrackOffset);
	scrollBarThumb.setPosition(scrollBarTrack.getPosition().x, scrollBarTrack.getPosition().y + relativeThumbPosY);

	// Update the scroll bar handle's position
	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*GlobalValues::getInstance().getWindow()));

	if (true/*scrollBarThumb.getGlobalBounds().contains(mousePosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left)*/) {
		isGrabbed = true;
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		isGrabbed = false;
	}

	if (isGrabbed) {

		// Calculate the new y position of the scroll bar thumb, keeping it within the bounds of the scroll bar track
		float newY = (mousePosition.y - menuView.getCenter().y - menuView.getSize().y / 2);
		newY = std::max(newY, scrollBarTrack.getPosition().y);
		newY = std::min(newY, scrollBarTrack.getPosition().y + (menuView.getSize().y - scrollBarThumb.getGlobalBounds().height));
		scrollBarThumb.setPosition(scrollBarThumb.getPosition().x, newY);
		relativeThumbPosY = newY - scrollBarTrack.getPosition().y;

		// Update the view
		float scrollPercentage = (newY - scrollBarTrack.getPosition().y) / (scrollBarTrack.getGlobalBounds().height);
		float viewPercentage = (interactableMenuLength - menuView.getSize().y) * scrollPercentage;
		
		std::cout << "Grabbed" << std::endl;

	}
}

void ScrollBar::update(sf::Vector2f menuPosition) {
	GlobalValues::getInstance().getWindow()->setView(menuView);
	updateInteractablePositions(menuPosition);
	updateScrollBarPositions(menuPosition);
	std::cout << "Thumb Y: " << std::to_string(scrollBarThumb.getPosition().y) << std::endl;
}

void ScrollBar::draw() {
	// Draw the scroll bar and handle
	GlobalValues::getInstance().getWindow()->draw(scrollBarTrack);
	GlobalValues::getInstance().getWindow()->draw(scrollBarThumb);
}