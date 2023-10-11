#include "Pellet.h"
#include "Circle.h"
#include "Map.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>



class Hashmap {
public:
    Hashmap(Map& map, sf::RenderWindow& window);
    void assignPellet(Pellet& pellet, Map& map);
    std::vector<Pellet*> checkCollision(Circle& circle, Map& map);
private:
    std::unordered_map<std::string, std::vector<Pellet*>> hashmap;
    std::string generateKey(int x, int y) const;
    std::vector<Pellet*> getPelletsInSameCell(float x, float y, Map& map) const;
};