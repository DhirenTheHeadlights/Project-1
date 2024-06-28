#pragma once

#include <SFML/Graphics.hpp>

namespace PirateGame {
    class WaterTiler {
    public:
        WaterTiler() {}

        void initialize() {
            if (!waterTileTexture.loadFromFile("PirateGameSprites/water_tiles_sheet.png")) {
                throw std::runtime_error("Failed to load water tile texture");
            }
            waterTileTexture.setRepeated(true);

            // Assuming each frame is of equal size and laid out horizontally in the texture
            frameWidth = waterTileTexture.getSize().x / numFrames;
            frameHeight = waterTileTexture.getSize().y;

            waterTileSprite.setTexture(waterTileTexture);
            waterTileSprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
            waterTileSprite.setScale(scalingFactor, scalingFactor); // Apply scaling factor
        }

        void update() {
            sf::Time deltaTime = frameClock.restart();
            elapsedTime += deltaTime;

            if (elapsedTime >= frameTime) {
                elapsedTime = sf::Time::Zero;

                if (currentFrame == numFrames - 1) {
                    currentFrame = 0;
                }
                else {
                    currentFrame++;
                }

                waterTileSprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            }
        }

        void draw(sf::RenderWindow* window) {
            sf::Vector2f viewCenter = window->getView().getCenter();
            sf::Vector2f viewSize = window->getView().getSize();
            sf::Vector2f viewPosition = viewCenter - (viewSize / 2.0f);

            int scaledFrameWidth = static_cast<int>(frameWidth * scalingFactor);
            int scaledFrameHeight = static_cast<int>(frameHeight * scalingFactor);

            int startX = static_cast<int>(viewPosition.x) / scaledFrameWidth - 1;
            int startY = static_cast<int>(viewPosition.y) / scaledFrameHeight - 1;
            int endX = static_cast<int>((viewPosition.x + viewSize.x)) / scaledFrameWidth + 1;
            int endY = static_cast<int>((viewPosition.y + viewSize.y)) / scaledFrameHeight + 1;

            for (int x = startX; x <= endX; ++x) {
                for (int y = startY; y <= endY; ++y) {
                    waterTileSprite.setPosition(x * scaledFrameWidth, y * scaledFrameHeight);
                    window->draw(waterTileSprite);
                }
            }
        }

    private:
        sf::Texture waterTileTexture;
        sf::Sprite waterTileSprite;

        int numFrames = 4;
        int frameWidth = 0;
        int frameHeight = 0;
        float scalingFactor = 4.0f;

        sf::Clock frameClock;
        sf::Time frameTime = sf::milliseconds(500);
        sf::Time elapsedTime = sf::Time::Zero;
        int currentFrame = 0;
    };
}
