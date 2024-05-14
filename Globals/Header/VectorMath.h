#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

namespace vm {
	inline float length(const sf::Vector2f& v) {
		return std::sqrt(v.x * v.x + v.y * v.y);
	}

	inline sf::Vector2f normalize(const sf::Vector2f& v) {
		float len = length(v);
		if (len != 0) {
			return sf::Vector2f(v.x / len, v.y / len);
		}
		return v;
	}

	inline float distance(const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return length(v1 - v2);
	}

	inline float dot(const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	inline float angle(const sf::Vector2f& v1, const sf::Vector2f& v2) {
		return std::acos(dot(v1, v2) / (length(v1) * length(v2)));
	}

	inline sf::Vector2f limit(const sf::Vector2f& v, float max) {
		if (length(v) > max) {
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

	inline float normalizeAnlge(float angle, float min = 0.f, float max = 360.f) {
		while (angle < min) angle += 360;
		while (angle >= max) angle -= 360;
		return angle;
	}
}