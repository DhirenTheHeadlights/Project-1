#pragma once

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "Ship_PG.h"
#include "LandMass_PG.h"

/// This class will handle the minimap for the game.

namespace PirateGame {
	class Minimap {
	public:
		Minimap() {
			// Load the textures
			if (!minimapTexture.loadFromFile("PirateGameSprites/minimap_sprite.png")) std::cout << "Error Loading File";
			else minimapSprite.setTexture(minimapTexture);
			minimapSprite.setScale(1.6f, 1.6f);

			if (!shipIconTexture.loadFromFile("PirateGameSprites/minimap_ship_icon.png")) std::cout << "Error Loading File";
			else shipIconSprite.setTexture(shipIconTexture);
			shipIconSprite.setScale(0.02f, 0.02f);
		};
		~Minimap() {};

		void draw();
		void update(float shipRotation);

		// Setters
		void setMinimapRadius(float size) { this->size = size; }
		void setLandmasses(std::vector<LandMass*>& landmasses) { this->landmasses = landmasses; }
		void setShip(Ship* ship) { this->ship = ship; }
		void setShips(std::vector<Ship*>& ships) { this->ships = ships; }
		void setMinimapPosition(sf::Vector2f position) { this->position = position; }
		
		// Getters
		sf::Sprite& getMinimapSprite() { return minimapSprite; }
		sf::Texture& getMinimapTexture() { return minimapTexture; }
		sf::CircleShape& getMinimap() { return minimap; }
		float getMinimapRadius() { return size; }
		sf::Vector2f getMinimapPosition() { return position; }
	private:
		// Values
		float size = 0;
		sf::Vector2f position;
		sf::Vector2f shipIconSize = { 6.f, 6.f };
		sf::Vector2f rockIconSize = { 2.f, 2.f };
		sf::Vector2f islandIconSize = { 4.f, 4.f };

		Ship* ship = nullptr;

		sf::Sprite minimapSprite;
		sf::Texture minimapTexture;
		
		sf::Sprite shipIconSprite;
		sf::Texture shipIconTexture;

		// Temp rectangle for minimap
		sf::CircleShape minimap;
		sf::RectangleShape shipIcon; // Temp

		std::vector<Ship*> ships;
		std::vector<LandMass*> landmasses;

		std::vector<sf::RectangleShape> visibleLandmassRects; // Landmasses on the minimap
		std::vector<sf::RectangleShape> shipsRects; // Ships on the minimap
	};
}