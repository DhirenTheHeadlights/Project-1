#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Map.h"
#include "Pellet.h"
#include "Circle.h"
#include <variant>
#include "Globals.h"

struct GridObject {
    std::variant<Pellet, Circle> object;
};

class Hashmap {
public:
    Hashmap();
    void assignPellet(const Pellet& pellet);
    void assignCircle(const Circle& circle);
    void checkCollision(Circle& circle, Hashmap hashmap);
    std::vector<Pellet> getPelletsInSameCell(float x, float y) const;
private:
    std::unordered_map<std::string, std::vector<GridObject>> hashmap;
    std::string generateKey(int x, int y) const;
};