#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

namespace PirateGame {
    class WindController {
    public:
        WindController() {
            randomizeWind();
        }

        void update() {
            sf::Time elapsed = windChangeTimer.getElapsedTime();
            float deltaTime = this->deltaTime.restart().asSeconds();
            if (elapsed > windChangeTime) {
                randomizeWind();
                windChangeTimer.restart();
            }
            else {
                // Gradually adjust current wind towards the target
                if (std::fabs(targetWindSpeed - windSpeed) > windSpeedChange) {
                    windSpeed += windSpeedChange * ((targetWindSpeed > windSpeed) ? 1 : -1) * deltaTime;
                }

                // Gradually adjust direction
                sf::Vector2f directionChange = normalize(targetWindDirection - windDirection) * windDirectionChange * deltaTime;
                windDirection += directionChange;
                windDirection = normalize(windDirection); // Ensure it stays normalized
            }
        }

        // Setters
        void setWindDirection(sf::Vector2f windDirection) { this->windDirection = normalize(windDirection); }
        void setWindSpeed(float windSpeed) { this->windSpeed = std::min(windSpeed, windSpeedMax); }

        // Getters
        sf::Vector2f getWindDirection() const { return windDirection; }
        float getWindSpeed() const { return windSpeed; }

        std::string getWindDirectionString() const {
            // Calculate the angle in degrees from the wind direction vector
            // No need to adjust for SFML's Y-axis, atan2 already handles the inversion.
            float angle = std::atan2(windDirection.y, windDirection.x) * 180 / 3.141526f;
            // Normalize the angle to be within the range [0, 360)
            if (angle < 0) angle += 360;

            // SFML's 0 degrees is East, so we map angles accordingly
            if (angle <= 22.5 || angle > 337.5) return "E";
            if (angle > 22.5 && angle <= 67.5) return "SE";
            if (angle > 67.5 && angle <= 112.5) return "S";
            if (angle > 112.5 && angle <= 157.5) return "SW";
            if (angle > 157.5 && angle <= 202.5) return "W";
            if (angle > 202.5 && angle <= 247.5) return "NW";
            if (angle > 247.5 && angle <= 292.5) return "N";
            if (angle > 292.5 && angle <= 337.5) return "NE";

            // Fallback, should never reach here
            return "Unknown";
        }

        sf::VertexArray getWindDirectionIndicator(const sf::Vector2f& startPosition, float scale = 1.0f) const {
            sf::VertexArray windDirectionIndicator(sf::Lines, 2);
            // Start position of the line
            windDirectionIndicator[0].position = startPosition;
            // End position calculated based on wind direction and speed (optionally scaled)
            windDirectionIndicator[1].position = startPosition + (windDirection * windSpeed * scale);

            // Set the color of the line
            windDirectionIndicator[0].color = sf::Color::White;
            windDirectionIndicator[1].color = sf::Color::White;

            return windDirectionIndicator;
        }

    private:
        sf::Vector2f windDirection = sf::Vector2f(0.f, 0.f);
        sf::Vector2f targetWindDirection = sf::Vector2f(0.f, 0.f);
        float windSpeed = 0.f;
        float targetWindSpeed = 0.f;

        float windSpeedChange = 0.5f; // Adjust for smoother transition
        float windDirectionChange = 0.1f;
        float windSpeedMax = 100.f;

        sf::Clock windChangeTimer;
        sf::Time windChangeTime = sf::seconds(10.f);
        sf::Clock deltaTime;

        void randomizeWind() {
            targetWindDirection = sf::Vector2f(static_cast<float>(rand() % 3 - 1), static_cast<float>(rand() % 3 - 1));
            targetWindSpeed = static_cast<float>(rand() % static_cast<int>(windSpeedMax) + 1);
        }

        sf::Vector2f normalize(sf::Vector2f vector) {
            float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
            if (length == 0.f) return sf::Vector2f(0.f, 0.f);
            return vector / length;
        }
    };
}
