#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

namespace vm {
	const float PI = 3.14159265f;

	inline float magnitude(const sf::Vector2f& v) {
		return std::sqrt(v.x * v.x + v.y * v.y);
	}

	inline float vectorToAngle(const sf::Vector2f& v) {
		return std::atan2(v.y, v.x) * 180.f / PI;
	}

	inline float degreesToRadians(float degrees) {
		return degrees * PI / 180.f;
	}

	inline float radiansToDegrees(float radians) {
		return radians * 180.f / PI;
	}

	inline sf::Vector2f normalize(const sf::Vector2f& v) {
		float len = magnitude(v);
		if (len != 0) {
			return sf::Vector2f(v.x / len, v.y / len);
		}
		return v;
	}

	inline float distance(const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return magnitude(v1 - v2);
	}

	inline float dot(const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	inline float angleBetweenVectorsDegrees(const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return std::acos(dot(v1, v2) / (magnitude(v1) * magnitude(v2)));
	}

	inline sf::Vector2f limit(const sf::Vector2f& v, float max) {
		if (magnitude(v) > max) {
			return normalize(v) * max;
		}
		return v;
	}

	inline sf::VertexArray createVector(const sf::Vector2f& position, const sf::Vector2f& direction, const sf::Color& color) {
		sf::VertexArray vector(sf::Lines, 2);
		vector[0].position = position;
		vector[0].color = color;
		vector[1].position = position + direction;
		vector[1].color = color;
		return vector;
	}

	inline float normalizeAngle(float angle, float min = 0.f, float max = 360.f) {
		while (angle < min) angle += 360;
		while (angle >= max) angle -= 360;
		return angle;
	}

	inline float clampAngleInDegrees(float angle, float refAngle, float maxOffset) {
		float diff = angle - refAngle;
		if (diff > maxOffset) return refAngle + maxOffset;
		if (diff < -maxOffset) return refAngle - maxOffset;
		return angle;
	}

	inline sf::Vector2f rotateAngleInDegrees(const sf::Vector2f& vector, float angleInDeg) {
		float rad = angleInDeg * PI / 180.0f;
		return sf::Vector2f(
			vector.x * std::cos(rad) - vector.y * std::sin(rad),
			vector.x * std::sin(rad) + vector.y * std::cos(rad)
		);
	}

	inline float angleBetweenVectorsRadians(const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return angleBetweenVectorsDegrees(v1, v2) * PI / 180.f;
	}
}