//
// Created by Teo on 2026-11-24.
//

#ifndef RIDICULOENGINE_ENTITYMANAGER_H
#define RIDICULOENGINE_ENTITYMANAGER_H
#include <array>
#include <queue>

#include "Entity.h"

/*
 * EntityManager is in charge of distributing IDs and keeping record of which IDs are in use,
 * and which are not.
 */
class EntityManager {
public:
    EntityManager();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    void SetSignature(Entity entity, Signature signature);
    Signature GetSignature(Entity entity);
private:
    // Queue of unused entity IDs
    std::queue<Entity> availableEntities;

    // Array of signatures where the index corresponds to the entity ID
    std::array<Signature, MAX_ENTITIES> signatures;

    // Total living entities - used to keep limits on how many exist
    uint32_t livingEntityCount{};
};


#endif //RIDICULOENGINE_ENTITYMANAGER_H
