#pragma once

/// This class is intended to be used by menu classes to create a scroll bar to handle the display of multiple interactables,
/// either in a vertical or horizontal orientation.

#include <SFML/Graphics.hpp>

#include "GlobalContext_PG.h"

#include "Interactable_PG.h"

namespace PirateGame {
	class ScrollBar {
	public:
		ScrollBar(GlobalContext& context, float spacingBetweenInteractables = 0.f) noexcept : context(context), spacing(spacingBetweenInteractables) {
			scrollBarTrack.setTexture(Textures::interactableTextures.getScrollBarTrack());
			scrollBarThumb.setTexture(Textures::interactableTextures.getScrollBarThumb());
		}

		void setUpScrollBar(const sf::Vector2f& scrollBarTrackOffset, const float scrollBarTrackLength, const sf::Vector2f& interactablePositionOffset, const sf::Vector2f& size, const sf::Vector2f& scale);

		void update(const sf::Vector2f& menuPosition, const std::vector<std::shared_ptr<Interactable>>& interactables);
		void draw() const;

		// Setters
		void setSpacing(float spacing) noexcept { this->spacing = spacing; }
		void setTextures(const sf::Texture& scrollBarTexture, const sf::Texture& scrollBarHandleTexture) noexcept {
			scrollBarTrack.setTexture(scrollBarTexture);
			scrollBarThumb.setTexture(scrollBarHandleTexture);
		}
		void setVertical(bool vertical) noexcept { this->vertical = vertical; }
		void setScale(sf::Vector2f& scale) noexcept { this->scale = scale; }

		// Call this function to correctly space the interactables
		void setFirstBoxSize(sf::Vector2f topBoxBounds) noexcept { this->firstBoxSize = topBoxBounds; }


		// Getters
		sf::Sprite& getSprite() noexcept { return scrollBarTrack; }
		float getScrollPercentage() const noexcept { return scrollPercentage; }

	private:
		// Context
		GlobalContext& context;

		// Functions
		void updateInteractablePositions(const sf::Vector2f& menuPosition, const std::vector<std::shared_ptr<Interactable>>& interactables) const;
		void updateScrollBarPositions(const sf::Vector2f& mousePosition);

		sf::Vector2f scale;
		sf::Vector2f size;
		sf::Vector2f menuDelta;
		sf::Vector2f firstBoxSize;

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