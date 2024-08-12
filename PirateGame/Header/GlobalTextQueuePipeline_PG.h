#pragma once

/// Serves as a way to queue text to be displayed on the screen

#include <SFML/Graphics.hpp>
#include <queue>

namespace PirateGame {
	class GlobalTextQueuePipeline {
	public:
		void addTextToQueue(sf::Text text, sf::Time duration) {
			text.setCharacterSize(textSize);
			TextDuration textDuration(text, duration);
			textQueue.push_back(textDuration);
		}

		void updateTextQueue(sf::RenderWindow* window) {
			float textX = window->getView().getCenter().x + window->getSize().x / 2.f;
			float textY = window->getView().getCenter().y + window->getSize().y / 2.f;

			for (int i = 0; i < textQueue.size(); i++) {
				// Set the position of the text
				float adjustedTextX = textX - textQueue[i].text.getGlobalBounds().width - 4.f;
				float adjustedTextY = textY - textQueue[i].text.getGlobalBounds().height - 20.f - (i * textQueue[i].text.getGlobalBounds().height);

				textQueue[i].text.setPosition(adjustedTextX, adjustedTextY);

				if (textQueue[i].duration <= textQueue[i].clock.getElapsedTime()) {
					textQueue.pop_front();
				}
			}
		}

		void drawTextQueue(sf::RenderWindow* window) {
			for (int i = 0; i < textQueue.size(); i++) {
				window->draw(textQueue[i].text);
			}
		}
	private:
		const int maxTextVisible = 5;
		const int textSize = 20;

		// Struct to hold text and duration
		struct TextDuration {
			sf::Text text;
			sf::Time duration;
			sf::Clock clock;

			TextDuration(sf::Text input, sf::Time duration)
				: duration(duration), text(input) {
				text.setFillColor(sf::Color::White);
				this->text = text;
			}
		};

		// Queue to hold text and duration
		std::deque<TextDuration> textQueue = {};
	};
}