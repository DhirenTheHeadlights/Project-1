#pragma once

#include <random>
#include <unordered_map>

#include "GlobalValues_PG.h"
#include "LandMassTypeGenerator_PG.h"


namespace PirateGame {
	
	class RegionHandler {
	public:
		RegionHandler() {};
		Region generateRegionType(sf::Vector2f position) {
			float distancefromorigin = vm::magnitude(position);
			if (distancefromorigin < 1000.f) {
				return Region::SandyShores;
			}
			else if (distancefromorigin < 2000.f) {
				return Region::MurkyMarshes;
			}
			else if (distancefromorigin < 3000.f) {
				return Region::DevilsDen;
			}
			else {
				return Region::ShipwreckShoals;
			}	
		}

		// Getters
		RegionValues getRegionValuesAtPosition(sf::Vector2f position) {
			Region region = generateRegionType(position);
			return RegionValues(region);
		}	
	private:
		/*std::unordered_map<std::string, Region> enumMap = {
		{"Island1", Island1},
		{"Island2", Island2},
		{"Ship", Ship}
		};*/
	};

}