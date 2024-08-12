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
				scaling = 1.f;
				break;
			case Region::MurkyMarshes:
				displayString = "Murky Marshes";
				scaling = 1.2f;
				break;
			case Region::DevilsDen:
				displayString = "Devil's Den";
				scaling = 1.5f;
				break;
			case Region::ShipwreckShoals:
				displayString = "Shipwreck Shoals";
				scaling = 2.f;
				break;
			}
		}
		Region region = Region::SandyShores;
		float scaling = 1;
		std::string displayString = "Sandy Shores";
	};
}