#pragma once

/// Simple factory class for creating world objects

#include "WorldDebug_PG.h"
#include "WorldDefault_PG.h"

namespace PirateGame {
	enum class WorldType {
		Default,
		DefaultDebug,
		LandmassAvoidanceTest,
		ShipCombatTest,
	};

	class SelectionButton {
	public:
		SelectionButton(const WorldType WT, const std::string& text) : worldType(WT) {
			buttonText.setFont(*GlobalFontHandler::getInstance().getGlobalFont());
			buttonText.setString(text);
			buttonText.setCharacterSize(24);
			buttonText.setFillColor(sf::Color::White);
		}

		void setPosition(float x, float y) {
			buttonText.setPosition(x, y);
		}

		void draw(sf::RenderWindow* window) {
			window->draw(buttonText);
		}

		bool isMouseOver(sf::RenderWindow* window) {
			return buttonText.getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
		}

		const std::string& getText() const { return buttonText.getString(); }
		const WorldType getWorldType() const { return worldType; }
	private:
		sf::Text buttonText;
		WorldType worldType;
	};

	class WorldFactory {
	public:
		WorldFactory() {
			// Add buttons for each world type
			worldTypes.push_back(SelectionButton(WorldType::Default, worldTypeToString(WorldType::Default)));
			worldTypes.push_back(SelectionButton(WorldType::DefaultDebug, worldTypeToString(WorldType::DefaultDebug)));

			// Set the position of the buttons
			for (auto& button : worldTypes) {
				if (y > yPadding * numButtonsInColumn) {
					y = 100.f; // Reset y position
					x += xPadding;
				}
				button.setPosition(x, y);
				y += yPadding;
			}
		}

		World* selectWorld(sf::RenderWindow* window) {
			window->clear();

			for (auto& button : worldTypes) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					window->close();
				}

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && button.isMouseOver(window)) {
					hasSelectedWorld = true;
					return createWorld(button.getWorldType(), window);
				}

				button.draw(window);
			}

			window->display();
		}

		static std::string worldTypeToString(WorldType worldType) {
			switch (worldType) {
			case WorldType::Default:
				return "Default";
			case WorldType::DefaultDebug:
				return "Default Debug";
			default:
				return "Unknown";
			}
		}

		bool getHasSelectedWorld() const { return hasSelectedWorld; }

	private:
		static World* createWorld(WorldType worldType, sf::RenderWindow* window) {
			switch (worldType) {
			case WorldType::Default:
				return new DefaultWorld(window);
			case WorldType::DefaultDebug:
				return new DefaultDebugWorld(window);
			default:
				return nullptr;
			}
		}

		std::vector<SelectionButton> worldTypes;

		bool hasSelectedWorld = false;

		float x = 100.f;                // X position of the first button
		float y = 100.f;                // Y position of the first button
		float yPadding = 150.f;         // Space between buttons
		float xPadding = 400.f;         // Space between columns
		float numButtonsInColumn = 6.f; // Number of buttons in a column
	};
}
