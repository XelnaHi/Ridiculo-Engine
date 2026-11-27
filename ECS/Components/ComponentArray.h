//
// Created by Teo on 2026-11-24.
//

#ifndef RIDICULOENGINE_COMPONENTARRAY_H
#define RIDICULOENGINE_COMPONENTARRAY_H
#include <cassert>
#include <unordered_map>

#include "../Entity/Entity.h"

class IComponentArray {
public:
    virtual ~IComponentArray() = default;

    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
    void InsertData(Entity entity, T component) {
        assert(
            mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() &&
            "Component added to same entity more than once.");


        // Put the new entry at the end and update the maps
        size_t newIndex = mSize;
        mEntityToIndexMap[entity] = newIndex;
        mIndexToEntityMap[newIndex] = entity;
        mComponentArray[newIndex] = component;
        ++mSize;
    }

    void RemoveData(Entity entity) {
        assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

        // Copy element at end into deleted element's place to maintain contiguous data layout (density)
        size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
        size_t indexOfLastEntity = mSize -1;
        mComponentArray[indexOfLastEntity] = mComponentArray[indexOfLastEntity];

        // Update map to point to moved spot
        Entity entityOfLastElement = mIndexToEntityMap[indexOfLastEntity];
        mEntityToIndexMap[entityOfLastElement] = indexOfLastEntity;
        mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        mEntityToIndexMap.erase(entity);
        mIndexToEntityMap.erase(indexOfLastEntity);

        --mSize;
    }

    // Get a reference to a component object so that we can perform operations on it directly.
    T& GetData(Entity entity) {
        assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Trying to retrieve non-existent component.");

        return mComponentArray[mEntityToIndexMap[entity]];
    }

    void EntityDestroyed(Entity entity) override {
        if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()) {
            RemoveData(entity);
        }
    }

private:
    std::array<T, MAX_ENTITIES> mComponentArray;

    // Map from Entity ID to an array index (represents which components an Entity "has")
    std::unordered_map<Entity, size_t> mEntityToIndexMap;

    // Map from an array index to an Entity ID (represents which Entity that owns this component)
    std::unordered_map<size_t, Entity> mIndexToEntityMap;

    // Total size of valid entries in the array
    size_t mSize{};
};
#endif //RIDICULOENGINE_COMPONENTARRAY_H
