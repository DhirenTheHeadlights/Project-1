#pragma once

/// Interactable textures for the game.

#include "TextureHandler_PG.h"

namespace PirateGame {
	class InteractableTextureHandler : public TextureHandler {
	public:
		InteractableTextureHandler() {
			blackGrayButton.loadFromFile("PirateGameSprites/Interactable_basic_gray_black.png");
			sliderThumb.loadFromFile("PirateGameSprites/slider_thumb.png");
			sliderTrack.loadFromFile("PirateGameSprites/slider_track.png");
			scrollBarThumb.loadFromFile("PirateGameSprites/scroll_bar_thumb.png");
			scrollBarTrack.loadFromFile("PirateGameSprites/scroll_bar_track_extendable.png");
		}
		~InteractableTextureHandler() {};

		// Getters
		sf::Texture& getBlackGrayButton() { return blackGrayButton; }
		sf::Texture& getSliderThumb() { return sliderThumb; }
		sf::Texture& getSliderTrack() { return sliderTrack; }
		sf::Texture& getScrollBarThumb() { return scrollBarThumb; }
		sf::Texture& getScrollBarTrack() { return scrollBarTrack; }

	private:
		sf::Texture blackGrayButton;
		sf::Texture sliderThumb;
		sf::Texture sliderTrack;
		sf::Texture scrollBarThumb;
		sf::Texture scrollBarTrack;
	};
}