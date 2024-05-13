#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <type_traits>
#include <random>

namespace PirateGame {
    template<typename EnumType>

    class TextureResourceManager {
        static_assert(std::is_enum<EnumType>::value, "TextureResourceManager requires an enum type as a template argument");

        // Custom hash function for enum class
        struct EnumHash {
            template<typename T>
            std::size_t operator()(T t) const {
                return static_cast<std::size_t>(t);
            }
        };
    public:
        bool loadTexture(EnumType type, const std::string& filename) {
            sf::Texture texture;
            if (!texture.loadFromFile(filename)) {
                std::cerr << "Error loading texture: " << filename << std::endl;
                return false;
            }

            textures[type] = std::move(texture);
            images[type] = textures[type].copyToImage();
            std::cout << "Loaded and stored texture " << filename << " for type " << static_cast<int>(type) << std::endl;
            return true;
        }

        bool loadTextures(EnumType type, std::vector<std::string> filenames) {
			std::vector<sf::Texture> textureGroup;
            for (const auto& filename : filenames) {
				sf::Texture texture;
                if (!texture.loadFromFile(filename)) {
					std::cerr << "Error loading texture: " << filename << std::endl;
					return false;
				}
				textureGroup.push_back(texture);
			}

			textureGroups[type] = std::move(textureGroup);
			std::cout << "Loaded and stored texture group for type " << static_cast<int>(type) << std::endl;
			return true;
		}

        const sf::Texture& getTexture(EnumType type) const {
            std::cout << "Getting texture for type " << static_cast<int>(type) << std::endl;
            return textures.at(type);
        }

        const std::vector<sf::Texture>& getTextureGroup(EnumType type) const {
            std::cout << "Getting texture group for type " << static_cast<int>(type) << std::endl;
            std::cout << "Size of texture group: " << textureGroups.at(type).size() << std::endl;
			return textureGroups.at(type);
		}

        const sf::Texture& getRandomTexture() const {
            std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(0, textures.size() - 1);
			auto it = textures.begin();
			std::advance(it, dist(gen));
			return it->second;
        }

        const sf::Image& getImage(EnumType type) const {
            return images.at(type);
        }
    private:
        std::unordered_map<EnumType, sf::Texture, EnumHash> textures;
        std::unordered_map<EnumType, sf::Image, EnumHash> images;

        std::unordered_map<EnumType, std::vector<sf::Texture>, EnumHash> textureGroups;
        std::unordered_map<EnumType, std::vector<sf::Image>, EnumHash> imageGroups;
    };
}
