#pragma once

#include <unordered_map>

#include "Region_PG.h"

namespace PirateGame {
	
	class RegionHandler {
	public:
		RegionHandler() {};
		Region generateRegionType(sf::Vector2f position) {
			float distancefromorigin = vm::magnitude(position);
			if (distancefromorigin < 10000.f) {
				return Region::SandyShores;
			}
			else if (distancefromorigin < 20000.f) {
				return Region::MurkyMarshes;
			}
			else if (distancefromorigin < 40000.f) {
				return Region::DevilsDen;
			}
			else if (distancefromorigin < 60000.f) {
				return Region::ShipwreckShoals;
			}	
			else return Region::StormySeas;
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