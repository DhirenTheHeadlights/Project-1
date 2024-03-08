#pragma once

/// This is a base class for all texture handing in game. It is very
/// important to call this class as few times as possible, as it loads
/// all the textures into memory. Consider giving this class as a reference
/// to other classes that need textures, rather than creating a new instance.

#include <SFML/Graphics.hpp>
#include <vector>
#include <stdlib.h>

namespace PirateGame {
	class TextureHandler {
	public:
		TextureHandler() {};
		~TextureHandler() {};

		// For now, there are no methods that are consistent across the base classes.
		// However, when creating a new class for textures, it is important to include
		// the loading of all textures in the constructor, and the methods to return
		// the textures. This base class will stay here in case we need to add any
		// methods that are consistent across all texture classes.
	};
}