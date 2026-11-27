//
// Created by Teo on 2026-11-26.
//

#ifndef RIDICULOENGINE_SYSTEMMANAGER_H
#define RIDICULOENGINE_SYSTEMMANAGER_H
#include <cassert>
#include <memory>
#include <typeinfo>
#include <unordered_map>

#include "System.h"
#include "../Entity/Entity.h"

class SystemManager {
public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem() {
        const char *typeName = typeid(T).name();

        assert(mSystems.find(typeName) == mSystems.end() && "System already registered");

        // Create a pointer to the system and return it so it can be used outside of the class
        auto system = std::make_shared<T>();
        mSystems.insert({typeName, system});
        return system;
    }

    template<typename T>
    void SetSignature(Signature signature) {
        const char *typeName = typeid(T).name();

        assert(mSystems.find(typeName) != mSystems.end() && "Trying to set signature on an unregistered system.");

        // Set the signature for this system
        mSignatures.insert({typeName, signature});
    }

    void EntityDestroyed(Entity entity) {
        // Erase a destroyed entity from all system lists. Since mEntities is a set,
        // no "if (valid)" checks are needed. Duplicate inserts and Erase on non-existent elements are discarded.
        for (auto const &pair: mSystems) {
            auto const &system = pair.second;
            system->mEntities.erase(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature) {
        // Notify each system that an entity's signature changed. This matters because entity signatures
        // describe which components are attached to the entity, and all specified components required for a
        // system must be present within the entity, else the components of the entity will
        // not be affected by the system.
        for (auto const &pair: mSystems) {
            auto const &type = pair.first;
            auto const &system = pair.second;
            auto const &systemSignature = mSignatures[type];

            // If Entity signature matches System signature we add the Entity to the set of entities within the
            // system, which means the entity has components that will be affected by this specific system.

            // Remember, the 'AND' bitwise operator will compare the relevant bits (column comparison) and if both
            // bits are 1, the corresponding bit position becomes 1, but if one of them is 0, the result bit will be
            // set to 0. This gives us a way to make sure that the *Entities* signature matches the *System's* signature.
            if ((entitySignature & systemSignature) == systemSignature) {
                system->mEntities.insert(entity);
            }
            // Entity no longer contains all necessary components in order to be considered for updates from this system.
            // Remove it from the systems entity set.
            else {
                system->mEntities.erase(entity);
            }
        }
    }

private:
    // Map from system string pointer to a signature
    std::unordered_map<const char *, Signature> mSignatures{};

    // Map from system string pointer to a system pointer
    std::unordered_map<const char *, std::shared_ptr<System> > mSystems{};
};

#endif //RIDICULOENGINE_SYSTEMMANAGER_H
