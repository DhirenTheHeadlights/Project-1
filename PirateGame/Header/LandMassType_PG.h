#pragma once

/// Landmass types, mainly to know which texture is used

#include <random>

namespace PirateGame {
	enum class IslandType {
		Island1,
		Island2,
	};

	enum class RockType {
		Rock1,
		Rock2,
		Rock3,
		Rock4,
		Rock5,
		Rock6,
		Rock7,
		Rock8,
		Rock9,
	};

	enum class ShipwreckType {
		Shipwreck1,
	};

	enum class MiscType {
		Cannonball,
	};

	// This enum will be used to determine the land mass type
	enum class LandMassType { Island, Rock, Shipwreck };
}