//
// Created by Teo on 2026-11-24.
//

#include "EntityManager.h"

#include <cassert>

#include "Entity.h"

EntityManager::EntityManager() {
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        availableEntities.push(entity);
    }
}

Entity EntityManager::CreateEntity() {
    assert(livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

    // Take an ID from the front of the queue
    Entity id = availableEntities.front(); // Inspect the value of the first element in queue (gets a ref to it)
    availableEntities.pop(); // Remove the first-in-line element from the queue
    ++livingEntityCount;

    return id;

}

void EntityManager::DestroyEntity(Entity entity) {
    assert(livingEntityCount < MAX_ENTITIES && "Entity out of range or already destroyed");

    // Invalidate the destroyed entity's signature
    signatures[entity].reset();

    // Put the signature at the back of the queue
    availableEntities.push(entity);
    --livingEntityCount;
}

void EntityManager::SetSignature(Entity entity, Signature signature) {
    assert(entity < MAX_ENTITIES && "Entity out of range");

    // Put this entity's signature into the array
    signatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity) {
    assert(entity < MAX_ENTITIES && "Entity out of range");

    return signatures[entity];
}
