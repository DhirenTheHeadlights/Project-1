#pragma once

#include "Region_PG.h"

namespace PirateGame {
	
	class RegionHandler {
	public:
		RegionHandler()  {}
		Region generateRegionType(sf::Vector2f position) {
			const float distanceFromOrigin = vm::magnitude(position);
			if (distanceFromOrigin < 10000.f) {
				return Region::SandyShores;
			}
			if (distanceFromOrigin < 20000.f) {
				return Region::MurkyMarshes;
			}
			if (distanceFromOrigin < 40000.f) {
				return Region::DevilsDen;
			}
			if (distanceFromOrigin < 60000.f) {
				return Region::ShipwreckShoals;
			}	
			return Region::StormySeas;
		}

		// Setter
		void setRegionScaling(const std::vector<float>& regionScaling) {
			this->regionScaling = regionScaling;
		}

		// Getters
		RegionValues getRegionValuesAtPosition(const sf::Vector2f position) {
			Region region = generateRegionType(position);
			return { region, regionScaling };
		}	
	private:
		std::vector<float> regionScaling;
	};

}