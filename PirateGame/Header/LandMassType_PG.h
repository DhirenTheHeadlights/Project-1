#pragma once

/// Landmass types, mainly to know which texture is used

namespace PirateGame {
	enum class IslandType : uint8_t {
		Island1,
		Island2,
	};

	enum class RockType : uint8_t {
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

	enum class ShipwreckType : uint8_t {
		Shipwreck1,
	};

	enum class MiscType {
		Cannonball,
	};

	// This enum will be used to determine the land mass type
	enum class LandMassType : uint8_t { Island, Rock, Shipwreck };

	static constexpr int numIslands = 2;
	static constexpr int numRocks = 9;
	static constexpr int numShipwrecks = 1;
}