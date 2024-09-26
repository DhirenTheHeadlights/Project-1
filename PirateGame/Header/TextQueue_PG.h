#pragma once

/// Serves as a way to queue text to be displayed on the screen

#include <SFML/Graphics.hpp>
#include <queue>

#include "Font_PG.h"
#include "Globals_PG.h"

namespace PirateGame {
	namespace TextQueue {

		inline constexpr int maxTextVisible = 5;
		inline constexpr int textSize = 20;
		inline int textScalingFactor = 750;
		inline std::unordered_map<std::string, sf::Text> textCache;

		// Struct to hold text and duration
		struct TextDuration {
			sf::Text text;
			sf::Time duration;
			sf::Clock clock;

			TextDuration(sf::Text input, const sf::Time duration)
				: text(std::move(input)), duration(duration) {
				text.setFillColor(sf::Color::White);

			}
		};
		// Queue to hold text and duration
		inline std::deque<TextDuration> textQueue = {};


		inline void addTextToQueue(sf::Text text, const sf::Time duration) {
			text.setCharacterSize(textSize);
			const TextDuration textDuration(text, duration);
			textQueue.push_back(textDuration);
		}

		inline void updateTextQueue(const sf::RenderWindow* window) {
			const float textX = window->getView().getCenter().x + static_cast<float>(window->getSize().x) / 2.f;
			const float textY = window->getView().getCenter().y + static_cast<float>(window->getSize().y) / 2.f;

			for (int i = 0; i < textQueue.size(); i++) {
				// Set the position of the text
				const float adjustedTextX = textX - textQueue[i].text.getGlobalBounds().width - 4.f;
				const float adjustedTextY = textY - textQueue[i].text.getGlobalBounds().height - 20.f - i * textQueue[i].text.getGlobalBounds().height;

				textQueue[i].text.setPosition(adjustedTextX, adjustedTextY);

				if (textQueue[i].duration <= textQueue[i].clock.getElapsedTime()) {
					textQueue.pop_front();
				}
			}
		}

		inline void drawTextQueue(sf::RenderWindow* window) {
			for (auto& i : textQueue) {
				window->draw(i.text);
			}
		}

		inline void displayText(const std::string& text, const sf::Vector2f& position, const sf::Color& color, int size = 30) {
			// Generate a key based on text and size for caching
			const std::string key = text + "_" + std::to_string(size);

			// Find or create the text object
			sf::Text& displayText = textCache[key];
			if (displayText.getString().isEmpty()) {
				displayText.setFont(Font::timesNewRoman);
				displayText.setString(text);
				displayText.setCharacterSize(size);
				displayText.setFillColor(color);
				displayText.setPosition(position);
			}

			// Update the position and size of the text
			displayText.setPosition(position);
			displayText.setCharacterSize(size * static_cast<int>(Globals::window->getView().getSize().y) / textScalingFactor);

			// Only draw the text if the view scale is small enough
			if (Globals::window->getView().getSize().x / static_cast<float>(Globals::window->getSize().x) < 5) {
				Globals::window->draw(displayText);
			}
		}
		
	};
}