#pragma once

namespace PirateGame {
	enum class Region {
		SandyShores, //0
		MurkyMarshes, //1
		DevilsDen, //3
		ShipwreckShoals, //4
		StormySeas //5
	};

	struct RegionValues {
		RegionValues(const Region region, const std::vector<float>& scalingVector) : region(region) {
			switch (region) {
			case Region::SandyShores:
				displayString = "Sandy Shores";
				scaling = scalingVector[0];
				break;
			case Region::MurkyMarshes:
				displayString = "Murky Marshes";
				scaling = scalingVector[1];
				break;
			case Region::DevilsDen:
				displayString = "Devil's Den";
				scaling = scalingVector[2];
				break;
			case Region::ShipwreckShoals:
				displayString = "Shipwreck Shoals";
				scaling = scalingVector[3];
				break;
			case Region::StormySeas:
				displayString = "Stormy Seas";
				scaling = scalingVector[4];
				break;
			
			}
		}
		Region region = Region::SandyShores;
		float scaling = 1;
		std::string displayString = "Sandy Shores";
	};
}