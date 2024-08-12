#pragma once

/// This class is intended to be used by menu classes to create a scroll bar to handle the display of multiple interactables,
/// either in a vertical or horizontal orientation.

#include <SFML/Graphics.hpp>

#include "GlobalContext_PG.h"

#include "Interactable_PG.h"

namespace PirateGame {
	class ScrollBar {
	public:
		ScrollBar(GlobalContext& context, float spacingBetweenInteractables = 0.f) : context(context), spacing(spacingBetweenInteractables) {
			scrollBarTrack.setTexture(context.GTH->getInteractableTextures().getScrollBarTrack());
			scrollBarThumb.setTexture(context.GTH->getInteractableTextures().getScrollBarThumb());
		}

		void setUpScrollBar(sf::Vector2f scrollBarTrackOffset, float scrollBarTrackLength, sf::Vector2f interactablePositionOffset, sf::Vector2f size, sf::Vector2f scale);

		void update(sf::Vector2f menuPosition);
		void draw() const;

		// Setters
		void setSpacing(float spacing) { this->spacing = spacing; }
		void setInteractables(std::vector<std::shared_ptr<Interactable>>& interactables) { this->interactables = interactables; }
		void setTextures(const sf::Texture& scrollBarTexture, const sf::Texture& scrollBarHandleTexture) {
			scrollBarTrack.setTexture(scrollBarTexture);
			scrollBarThumb.setTexture(scrollBarHandleTexture);
		}
		void setVertical(bool vertical) { this->vertical = vertical; }
		void setScale(sf::Vector2f scale) { this->scale = scale; }

		// Getters
		sf::Sprite& getSprite() { return scrollBarTrack; }

	private:
		// Context
		GlobalContext& context;

		// Functions
		void updateInteractablePositions(sf::Vector2f menuPosition);
		void updateScrollBarPositions(sf::Vector2f mousePosition);

		// Vars
		std::vector<std::shared_ptr<Interactable>> interactables;

		sf::Vector2f scale;
		sf::Vector2f size;
		sf::Vector2f menuDelta;

		sf::View menuView;

		float spacing;

		sf::Vector2f scrollBarTrackOffset;
		sf::Vector2f previousMenuPosition;
		float scrollBarTrackLength = 0.f;
		float scrollPercentage = 0.f;

		sf::Vector2f interactablePositionOffset;
		float interactableMenuLength = 0.f;

		sf::Sprite scrollBarTrack;
		sf::Sprite scrollBarThumb;

		bool isGrabbed = false;
		bool vertical = true;
	};
}