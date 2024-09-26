#pragma once

#include "GlobalIDManager_PG.h"
#include "Input_PG.h"
#include "GlobalSoundManager_PG.h"
#include "GlobalTextureHandler_PG.h"
#include "WindHandler_PG.h"
#include "JSONLoader_PG.h"

namespace PirateGame {
	struct GlobalContext {


		std::unique_ptr<GlobalSoundManager> GSM = std::make_unique<GlobalSoundManager>();
		std::unique_ptr<GlobalIDManager> GIDM = std::make_unique<GlobalIDManager>();
		std::unique_ptr<GlobalTextureHandler> GTH = std::make_unique<GlobalTextureHandler>();
		std::unique_ptr<JSONLoader> JSL = std::make_unique<JSONLoader>();

	};
}
