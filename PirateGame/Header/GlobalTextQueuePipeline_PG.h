#pragma once

/// Serves as a way to queue text to be displayed on the screen

#include <SFML/Graphics.hpp>
#include <queue>

#include "GlobalValues_PG.h"
#include "GlobalFontHandler_PG.h"

namespace PirateGame {
	class GlobalTextQueuePipeline {
	public:
		static GlobalTextQueuePipeline& getInstance() {
			static GlobalTextQueuePipeline instance;
			return instance;
		}

		void addTextToQueue(std::string text, sf::Time duration) {
			TextDuration textDuration(text, duration, textSize);
			textQueue.push_back(textDuration);
		}

		void updateTextQueue() {
			float textX = GlobalValues::getInstance().getWindow()->getView().getCenter().x + GlobalValues::getInstance().getWindow()->getSize().x / 2.f;
			float textY = GlobalValues::getInstance().getWindow()->getView().getCenter().y + GlobalValues::getInstance().getWindow()->getSize().y / 2.f;

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

		void drawTextQueue() {
			for (int i = 0; i < textQueue.size(); i++) {
				GlobalValues::getInstance().getWindow()->draw(textQueue[i].text);
			}
		}
	private:
		// Private constructor
		GlobalTextQueuePipeline() {};

		// Disable copy constructor and assignment operator
		GlobalTextQueuePipeline(GlobalTextQueuePipeline const&) = delete;
		void operator=(GlobalTextQueuePipeline const&) = delete;

		const int maxTextVisible = 5;
		const int textSize = 20;

		// Struct to hold text and duration
		struct TextDuration {
			sf::Text text;
			sf::Time duration;
			sf::Clock clock;

			TextDuration(std::string string, sf::Time duration, int textSize = 20)
				: duration(duration) {
				sf::Text text(string, *GlobalFontHandler::getInstance().getGlobalFont(), textSize);
				text.setFillColor(sf::Color::White);
				this->text = text;
			}
		};

		// Queue to hold text and duration
		std::deque<TextDuration> textQueue;
	};
}