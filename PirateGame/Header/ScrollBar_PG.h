#pragma once

/// This class is intended to be used by menu classes to create a scroll bar to handle the display of multiple interactables,
/// either in a vertical or horizontal orientation.

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "GlobalTextureHandler_PG.h"

#include "Interactable_PG.h"

namespace PirateGame {
	class ScrollBar {
	public:
		ScrollBar(float spacingBetweenInteractables = 0.f) : spacing(spacingBetweenInteractables) {
			scrollBarTrack.setTexture(GlobalTextureHandler::getInstance().getInteractableTextures().getScrollBarTrack());
			scrollBarThumb.setTexture(GlobalTextureHandler::getInstance().getInteractableTextures().getScrollBarThumb());
		}

		void setUpScrollBar(sf::Vector2f scrollBarTrackOffset, float scrollBarTrackLength, sf::Vector2f interactablePositionOffset, float interactableMenuLength);

		void update(sf::Vector2f menuPosition);
		void draw();

		// Setters
		void setSpacing(float spacing) { this->spacing = spacing; }
		void setInteractables(std::vector<std::unique_ptr<Interactable>>& interactables) { this->interactables = std::move(interactables); }
		void setTextures(const sf::Texture& scrollBarTexture, const sf::Texture& scrollBarHandleTexture) {
			scrollBarTrack.setTexture(scrollBarTexture);
			scrollBarThumb.setTexture(scrollBarHandleTexture);
		}
		void setVertical(bool vertical) { this->vertical = vertical; }
		void setScale(sf::Vector2f scale) { this->scale = scale; }
	private:
		// Vars
		std::vector<std::unique_ptr<Interactable>> interactables;

		sf::Vector2f scale;

		float spacing;

		sf::Vector2f scrollBarTrackOffset;
		float scrollBarTrackLength = 0.f;

		sf::Vector2f interactablePositionOffset;
		float interactableMenuLength = 0.f;

		sf::Sprite scrollBarTrack;
		sf::Sprite scrollBarThumb;

		bool isGrabbed = false;
		bool vertical = true;
	};
}