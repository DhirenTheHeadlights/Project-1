#pragma once

///  This class is the base class for all interactable objects in the game.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

#include "GlobalValues_PG.h"
#include "GlobalTextureHandler_PG.h"
#include "GlobalSoundManager_PG.h"
#include "GlobalFontHandler_PG.h"
#include "GlobalInputHandler_PG.h"

namespace PirateGame {
	class Interactable {
	public:
		Interactable() {
			window = GlobalValues::getInstance().getWindow();
			font = *GlobalFontHandler::getInstance().getGlobalFont();
		};

		~Interactable() {};

		void createInteractable(sf::Texture& texture, sf::Text& title, sf::Vector2f scale = { 1.f, 1.f });
		virtual void customInteractableSetUp() = 0;
		virtual void interact() = 0;
		virtual void draw();

		// Setters for properties
		virtual void setPosition(sf::Vector2f pos);
		void setTextPosition(sf::Vector2f pos) { text.setPosition(pos); }
		void setSoundId(SoundId id) { soundId = id; }

		// Getters for properties
		sf::Text& getText() { return text; }
		sf::Sprite& getSprite() { return sprite; }

	protected:
		// Objects
		sf::Sprite sprite;
		sf::Texture texture;
		sf::Font font;
		sf::Text text;
		sf::RenderWindow* window = nullptr;

		SoundId soundId = SoundId::Select;

		std::string name;

		float defaultTextSize = 20.0f;

		// Colors
		sf::Color textColor = sf::Color(0, 0, 0, 200); // Black
	};
}

