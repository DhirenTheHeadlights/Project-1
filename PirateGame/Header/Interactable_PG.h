#pragma once

///  This class is the base class for all interactable objects in the game.

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

#include "GlobalValues_PG.h"
#include "GlobalTextureHandler_PG.h"

namespace PirateGame {
	class Interactable {
	public:
		Interactable() { 
			window = GlobalValues::getInstance().getWindow();
			font = GlobalValues::getInstance().getFont();

			sprite.setTexture(GlobalTextureHandler::getInstance().getInteractableTextures().getBlackGrayButton());
			sprite.setScale(7.f, 7.f);
			sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
		};

		~Interactable() {};

		virtual void createInteractable(sf::Vector2f size);
		virtual void interact() = 0;
		virtual void draw();
		virtual void updateColors();

		// Setters for properties
		virtual void setPosition(sf::Vector2f pos);
		void setSize(sf::Vector2f size) { this->size = size; }
		void setScale(sf::Vector2f healthBarScale) { this->healthBarScale = healthBarScale; }
		void setString(const std::string& string) { this->name = string, text.setString(string); }

		void setTextPosition(sf::Vector2f pos) { text.setPosition(pos); }

		// Setters for colors
		void setBackgroundColor(sf::Color color) { this->backgroundColor = color; }
		void setForegroundColor(sf::Color color) { this->foregroundColor = color; }
		void setFrameColor(sf::Color color) { this->frameColor = color; }
		void setTextColor(sf::Color color) { this->textColor = color; }
		void setOutlineThickness(float thickness) { this->outlineThickness = thickness; }
		void setPadding(float padding) { this->padding = padding; }
		void setTexture(sf::Texture& texture) { this->texture = texture; sprite.setTexture(texture); }

		// Getters for properties
		sf::Text& getText() { return text; }
		sf::RectangleShape& getBackground() { return background; }
		sf::RectangleShape& getForeground() { return foreground; }
		sf::RectangleShape& getFrame() { return frame; }

		sf::Sprite& getSprite() { return sprite; }

		sf::Vector2f getPosition() const { return position; }
		sf::Vector2f getSize() const { return size; }
		float getPadding() const { return padding; }

	protected:
		// Variables
		sf::Vector2f position;
		sf::Vector2f size;
		sf::Vector2f healthBarScale;
		std::string name;

		float padding = 10.0f;
		float outlineThickness = 5.f;

		// Objects
		sf::Sprite sprite;
		sf::Texture texture;
		sf::RectangleShape background;
		sf::RectangleShape foreground;
		sf::RectangleShape frame;
		sf::Font font;
		sf::Text text;
		sf::RenderWindow* window = nullptr;

		// Default colors
		sf::Color backgroundColor = sf::Color(100, 100, 150, 200);
		sf::Color foregroundColor = sf::Color(255, 255, 255, 200);
		sf::Color frameColor = sf::Color(150, 150, 200, 200);
		sf::Color textColor = sf::Color(0, 0, 0, 200);
	};
}

