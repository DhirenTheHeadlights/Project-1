#pragma once


namespace PirateGame {
	enum class Region {
		SandyShores, //0
		MurkyMarshes, //1
		DevilsDen, //3
		ShipwreckShoals, //4
	};
	struct RegionValues {
		RegionValues(Region region) : region(region) {
			switch (region) {
			case Region::SandyShores:
				displayString = "Sandy Shores";
				scaling = 1;
				break;
			case Region::MurkyMarshes:
				displayString = "Murky Marshes";
				scaling = 1.2;
				break;
			case Region::DevilsDen:
				displayString = "Devil's Den";
				scaling = 1.5;
				break;
			case Region::ShipwreckShoals:
				displayString = "Shipwreck Shoals";
				scaling = 2;
				break;
			}
		}
		Region region;
		float scaling;
		std::string displayString;
	};
}