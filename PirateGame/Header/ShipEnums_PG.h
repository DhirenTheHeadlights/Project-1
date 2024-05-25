#pragma once

/// This class just holds the ship enums and properties.

#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace PirateGame {
	const int numShipClasses = 5;
	enum class ShipClass {
		Random,
		Sloop,
		Brigantine,
		Frigate,
		ManOWar,
		Galleon
	};

	enum class FiringSide { Port, Starboard };

	// Struct to hold ship properties
	struct ShipProperties {
		float baseSpeed = 1.f;
		float maxHealth = 100.f;
		float regenRate = 0.5f;
		float scaleX = 1.f, scaleY = 1.f;
		int numCannons = 1;
	};

	inline std::unordered_map<ShipClass, ShipProperties> ShipConfig = {
		{ ShipClass::Sloop,		 {100.f, 100.f, 1,     .1f,  .1f,  2}},
		{ ShipClass::Brigantine, {95.f,  133.f, 1.48f, .12f, .12f, 4}},
		{ ShipClass::Frigate,	 {82.f,  192.f, 2.15f, .15f, .15f, 6}},
		{ ShipClass::ManOWar,	 {77.f,  250.f, 3.f,   .18f, .18f, 8}},
		{ ShipClass::Galleon,	 {63.f,  380.f, 4.6f,  .23f, .23f, 10}}
	};

	inline ShipProperties getShipProperties(ShipClass shipClass) {
		return ShipConfig[shipClass];
	}

	inline ShipClass getRandomShipClass() {
		// Generate a random number between 1 and 5
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(1, 5); // 0 is not included
		return static_cast<ShipClass>(dis(gen));
	}

	inline std::string shipClassToString(ShipClass shipClass) {
		switch (shipClass) {
		case ShipClass::Sloop:
			return "Sloop";
		case ShipClass::Brigantine:
			return "Brigantine";
		case ShipClass::Frigate:
			return "Frigate";
		case ShipClass::ManOWar:
			return "Manowar";
		case ShipClass::Galleon:
			return "Galleon";
		default:
			return "Unkown";
		}
	}
}

