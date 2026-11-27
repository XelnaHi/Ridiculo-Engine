//
// Created by Teo on 2026-11-26.
//

#ifndef RIDICULOENGINE_COMPONENTMANAGER_H
#define RIDICULOENGINE_COMPONENTMANAGER_H
#include <memory>
#include <unordered_map>

#include "ComponentArray.h"
#include "typeinfo"
#include "../Entity/Entity.h"
/*
 *
 */
class ComponentManager {
    public:
    template <typename T>
    void RegisterComponent() {
        const char* typeName = typeid(T).name();

        assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Trying to register component type more than once.");

        // Add the component type to the component type map
        mComponentTypes.insert({typeName, mNextComponentType});

        // Create a ComponentArray pointer and add it to the component arrays map
        mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

        // Increment the value so that the next component registered will be assigned an updated (and unique) value.
        ++mNextComponentType;
    }

    template<typename T>
    ComponentType GetComponentType() {
        const char* typeName = typeid(T).name();

        assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before attempted usage.");

        return mComponentTypes[typeName];
    }


    template<typename T>
    T& GetComponent(Entity entity) {
      return GetComponentArray<T>()->GetData(entity);
    }


    template<typename T>
    void AddComponent(Entity entity, T component) {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        GetComponentArray<T>()->RemoveData(entity);
    }

    void EntityDestroyed(Entity entity) {
        for (auto const& pair : mComponentArrays) {
            auto const& comp = pair.second;

            comp->EntityDestroyed(entity);
        }
    }

private:
    // Map from type string pointer to a component type
    std::unordered_map<const char*, ComponentType> mComponentTypes{};

    // Map from type string pointer to a component array
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

    // The component type to be assigned to the next registered component, starting at 0.
    ComponentType mNextComponentType{};

    // Convenience function to get the statically casted pointer to the ComponentArray of type T.
    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray() {
        const char* typeName = typeid(T).name();

        assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before attempted usage.");

        return static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
    }
};

#endif //RIDICULOENGINE_COMPONENTMANAGER_H
