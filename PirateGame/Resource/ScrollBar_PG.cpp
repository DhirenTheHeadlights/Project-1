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
	menuView.setCenter(menuPosition + interactablePositionOffset + sf::Vector2f(menuView.getSize().x / 2.f, menuView.getSize().y * scrollPercentage));
    for (size_t i = 0; i < interactables.size(); ++i) {
        interactables[i]->setPosition(sf::Vector2f(menuView.getCenter().x - menuView.getSize().x / 2.f, i * (interactables[i]->getSprite().getGlobalBounds().height + spacing)));
	}
}

void ScrollBar::updateScrollBarPositions(sf::Vector2f menuPosition) {
    // Update the positions of the scroll bar track and thumb based on the new menu position
    scrollBarTrack.setPosition(menuPosition + scrollBarTrackOffset);
    scrollBarThumb.setPosition(scrollBarTrack.getPosition().x, scrollBarThumb.getPosition().y + menuDelta.y);

    // Check for bounds
    if (scrollBarThumb.getPosition().y < scrollBarTrack.getPosition().y) {
		scrollBarThumb.setPosition(scrollBarTrack.getPosition().x, scrollBarTrack.getPosition().y);
	}
	else if (scrollBarThumb.getPosition().y > scrollBarTrack.getPosition().y + (scrollBarTrackLength - scrollBarThumb.getGlobalBounds().height)) {
		scrollBarThumb.setPosition(scrollBarTrack.getPosition().x, scrollBarTrack.getPosition().y + (scrollBarTrackLength - scrollBarThumb.getGlobalBounds().height));
	}   

    // Get the mouse position in window coordinates
    sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*context.GV->getWindow()));
    sf::Vector2f worldPosition = context.GV->getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*context.GV->getWindow()));

    // Check if the scroll bar thumb is grabbed
    if (scrollBarThumb.getGlobalBounds().contains(worldPosition) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        isGrabbed = true;
    }
    else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        isGrabbed = false;
    }

    // If grabbed, update the thumb position
    if (isGrabbed) {
        float newY = worldPosition.y - scrollBarThumb.getGlobalBounds().height / 2;
        newY = std::max(newY, scrollBarTrack.getPosition().y);
        newY = std::min(newY, scrollBarTrack.getPosition().y + (menuView.getSize().y - scrollBarThumb.getGlobalBounds().height));
        scrollBarThumb.setPosition(scrollBarTrack.getPosition().x, newY);

        scrollPercentage = (newY - scrollBarTrack.getPosition().y) / (menuView.getSize().y - scrollBarThumb.getGlobalBounds().height);
    }

    // Update the previous menu position
    previousMenuPosition = menuPosition;
}



void ScrollBar::update(sf::Vector2f menuPosition) {
    // Calculate the change in menu position
    menuDelta = menuPosition - previousMenuPosition;
	updateScrollBarPositions(menuPosition);
	context.GV->getWindow()->setView(menuView);
	updateInteractablePositions(menuPosition);
}

void ScrollBar::draw() const {
	// Draw the scroll bar and handle
	context.GV->getWindow()->draw(scrollBarTrack);
	context.GV->getWindow()->draw(scrollBarThumb);
}