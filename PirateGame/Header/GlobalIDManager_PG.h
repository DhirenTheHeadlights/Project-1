#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

namespace PirateGame {
    struct ID {
    public:
        
        int id;

        ID() : id(-1) {}

        bool operator==(const ID& other) const {
			if (id == -1 || other.id == -1) {
				return false;
			}
            return id == other.id;
        }
    private:
        

        explicit ID(int id) : id(id) {}

        friend class GlobalIDManager;


    };

    class GlobalIDManager {
    public:
        std::shared_ptr<ID> generateID() {
            int newID = static_cast<int>(ids.size());
            auto idPtr = std::shared_ptr<ID>(new ID(newID)); // Create shared_ptr directly
            ids.push_back(idPtr);
            return idPtr;
        }

        void registerObject(std::shared_ptr<ID> obj) {
            idMap_[obj->id] = obj;
        }

        bool isObjectAlive(int id) {
            auto it = idMap_.find(id);
            if (it != idMap_.end()) {
                if (!it->second.expired()) {
                    return true;
                }
                else {
                    idMap_.erase(it);
                }
            }
            return false;
        }

    private:
        std::vector<std::shared_ptr<ID>> ids;
        std::unordered_map<int, std::weak_ptr<ID>> idMap_;
    };
}
