#include "ScrollBar_PG.h"

using namespace PirateGame;

void ScrollBar::setUpScrollBar(const sf::Vector2f& scrollBarTrackOffset, const float scrollBarTrackLength, const sf::Vector2f& interactablePositionOffset, const sf::Vector2f& size, const sf::Vector2f& scale) {
	this->scrollBarTrackOffset = scrollBarTrackOffset;
	this->scrollBarTrackLength = scrollBarTrackLength;
	this->interactablePositionOffset = interactablePositionOffset;
	this->interactableMenuLength = vertical ? size.y : size.x;

	// Set scales
	scrollBarThumb.setScale(scale);
	scrollBarTrack.setScale(scale);
}

void ScrollBar::updateInteractablePositions(const sf::Vector2f& menuPosition, const std::vector<std::shared_ptr<Interactable>>& interactables) const {
    float sumHeight = 0.0f;

    for (size_t i = 0; i < interactables.size(); ++i) {
        // The first box uses the size set from the inventory menu only if the 
        // scroll percentage is greater than the height of the first box
        if (i == 0) {
            sumHeight = vertical ? firstBoxSize.y : firstBoxSize.x;
        }
        else if (i > 0) {
            sumHeight += interactables[i - 1]->getSprite().getGlobalBounds().height;
        }

        // Adjust the position based on scroll percentage
        interactables[i]->setPosition(menuPosition + interactablePositionOffset + sf::Vector2f(0.f, sumHeight - scrollPercentage * interactableMenuLength + menuDelta.y));
    }
}

void ScrollBar::updateScrollBarPositions(const sf::Vector2f& menuPosition) {
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
        newY = std::min(newY, scrollBarTrack.getPosition().y + (interactableMenuLength - scrollBarThumb.getGlobalBounds().height));
        scrollBarThumb.setPosition(scrollBarTrack.getPosition().x, newY);

        scrollPercentage = (newY - scrollBarTrack.getPosition().y) / (interactableMenuLength - scrollBarThumb.getGlobalBounds().height);
    }

    // Update the previous menu position
    previousMenuPosition = menuPosition;
}

void ScrollBar::update(const sf::Vector2f& menuPosition, const std::vector<std::shared_ptr<Interactable>>& interactables) {
    // Calculate the change in menu position
    menuDelta = menuPosition - previousMenuPosition;
	updateScrollBarPositions(menuPosition);
	updateInteractablePositions(menuPosition, interactables);
}

void ScrollBar::draw() const {
	// Draw the scroll bar and handle
	context.GV->getWindow()->draw(scrollBarTrack);
	context.GV->getWindow()->draw(scrollBarThumb);
}