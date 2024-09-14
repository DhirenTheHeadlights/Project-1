#pragma once

/// One stop shop for all global context classes

#include "GlobalChunkHandler_PG.h"
#include "GlobalClock_PG.h"
#include "GlobalFontHandler_PG.h"
#include "GlobalGameStateManager_PG.h"
#include "GlobalIDManager_PG.h"
#include "GlobalInputHandler_PG.h"
#include "GlobalSoundManager_PG.h"
#include "GlobalTextQueuePipeline_PG.h"
#include "GlobalTextureHandler_PG.h"
#include "GlobalValues_PG.h"
#include "GlobalWindController_PG.h"
#include "JSONLoader_PG.h"

namespace PirateGame {
	struct GlobalContext {
		std::unique_ptr<GlobalFontHandler> GFH = std::make_unique<GlobalFontHandler>();
		std::unique_ptr<GlobalValues> GV = std::make_unique<GlobalValues>(*GFH->getGlobalFont());
		std::unique_ptr<GlobalChunkHandler> GCH = std::make_unique<GlobalChunkHandler>();
		std::unique_ptr<GlobalGameStateManager> GGSM = std::make_unique<GlobalGameStateManager>();
		std::unique_ptr<GlobalInputHandler> GIH = std::make_unique<GlobalInputHandler>();
		std::unique_ptr<GlobalWindController> GWC = std::make_unique<GlobalWindController>();
		std::unique_ptr<GlobalTextQueuePipeline> GTQP = std::make_unique<GlobalTextQueuePipeline>();
		std::unique_ptr<GlobalSoundManager> GSM = std::make_unique<GlobalSoundManager>();
		std::unique_ptr<GlobalClock> GC = std::make_unique<GlobalClock>();
		std::unique_ptr<GlobalIDManager> GIDM = std::make_unique<GlobalIDManager>();
		std::unique_ptr<GlobalTextureHandler> GTH = std::make_unique<GlobalTextureHandler>();
		std::unique_ptr<JSONLoader> JSL = std::make_unique<JSONLoader>();

	};
}
