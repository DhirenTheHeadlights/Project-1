#pragma once

#include <SFML/Graphics.hpp>

#include "GlobalValues_PG.h"
#include "PlayerShip_PG.h"
#include "EnemyShip_PG.h"
#include "LandMass_PG.h"
#include "HUDTextures_PG.h"

/// This class will handle the minimap for the game.

namespace PirateGame {
	class Minimap {
	public:
		Minimap() {
			// Load the textures
			minimapSprite.setTexture(textures.getMiniMap());
			minimapSprite.setScale(1.6f, 1.6f);

			shipIconSprite.setTexture(textures.getMiniMapShipIcon());
			shipIconSprite.setScale(0.02f, 0.02f);
		};
		~Minimap() {};

		void draw();
		void update(float shipRotation);

		// Setters
		void setMinimapRadius(float size) { this->size = size; }
		void setLandmasses(std::vector<std::shared_ptr<LandMass>>& landmasses) { this->landmasses = landmasses; }
		void setShip(Ship* ship) { this->ship = ship; }
		void setEnemyShips(std::vector<std::shared_ptr<EnemyShip>>& ships) { this->ships = ships; }
		void setMinimapPosition(sf::Vector2f position) { this->position = position; }
		
		// Getters
		sf::Sprite& getMinimapSprite() { return minimapSprite; }
		sf::CircleShape& getMinimap() { return minimap; }
		float getMinimapRadius() const { return size; }
		sf::Vector2f getMinimapPosition() const { return position; }
	private:
		// Values
		float size = 0;
		sf::Vector2f position;
		sf::Vector2f shipIconSize = { 6.f, 6.f };
		sf::Vector2f rockIconSize = { 2.f, 2.f };
		sf::Vector2f islandIconSize = { 4.f, 4.f };
		sf::Vector2f shipRectSize = { 3.f, 3.f };

		Ship* ship = nullptr;

		sf::Sprite minimapSprite;
		sf::Sprite shipIconSprite;

		HUDTextureHandler textures;

		// Temp rectangle for minimap
		sf::CircleShape minimap;
		sf::RectangleShape shipIcon; // Temp

		std::vector<std::shared_ptr<EnemyShip>> ships;
		std::vector<std::shared_ptr<LandMass>> landmasses;

		std::vector<sf::RectangleShape> visibleLandmassRects; // Landmasses on the minimap
		std::vector<sf::RectangleShape> visibleShipRects; // Ships on the minimap
	};
}