//
// Created by Teo on 2026-11-27.
//

#ifndef RIDICULOENGINE_COORDINATOR_H
#define RIDICULOENGINE_COORDINATOR_H
#include "../Components/ComponentManager.h"
#include "../Systems/SystemManager.h"
#include "../Entity/EntityManager.h"


class Coordinator {
    public:
    void Init() {
        mComponentManager = std::make_unique<ComponentManager>();
        mSystemManager = std::make_unique<SystemManager>();
        mEntityManager = std::make_unique<EntityManager>();
    }

    // Entity Methods
    Entity CreateEntity() {
        return mEntityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity) {
        mEntityManager->DestroyEntity(entity);
        mComponentManager->EntityDestroyed(entity);
        mSystemManager->EntityDestroyed(entity);
    }

    // Component Methods
    template<typename T>
    void RegisterComponent() {
        mComponentManager->RegisterComponent<T>();
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        mComponentManager->RemoveComponent<T>(entity);

        Signature signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), false);
        mEntityManager->SetSignature(entity, signature);

        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return mComponentManager->GetComponent<T>(entity);
    }


    template<typename T>
    ComponentType GetComponentType () {
        return mComponentManager->GetComponentType<T>();
    }

    // System Methods
    template<typename T>
    std::shared_ptr<T> RegisterSystem() {
        return mSystemManager->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature signature) {
        mSystemManager->SetSignature<T>(signature);
    }


private:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<SystemManager> mSystemManager;
};

#endif //RIDICULOENGINE_COORDINATOR_H
