//
// Created by Teo on 2026-11-27.
//

#ifndef RIDICULOENGINE_POSITIONSYSTEM_H
#define RIDICULOENGINE_POSITIONSYSTEM_H
#include <iostream>
#include <ostream>

#include "System.h"
#include "../Components/Transform.h"
#include "../ECS/Coordinator/Coordinator.h"
#include "../Components/Tag.h"

extern Coordinator gCoordinator;

class PositionSystem : public System {
public:
    void PrintPositionSystem() {
        for (auto const &entity: mEntities) {
            auto &transform = gCoordinator.GetComponent<Transform>(entity);
            auto &tag = gCoordinator.GetComponent<Tag>(entity);

            std::cout << "Entity with Tag: " << tag.tagName << " placed at (" << transform.Position.x << ", " <<
                    transform.Position.y << ", " << transform.Position.z << std::endl;
        }
    }
};

#endif //RIDICULOENGINE_POSITIONSYSTEM_H
