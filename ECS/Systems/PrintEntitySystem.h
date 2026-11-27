//
// Created by Teo on 2026-11-27.
//

#ifndef RIDICULOENGINE_PRINTENTITYSYSTEM_H
#define RIDICULOENGINE_PRINTENTITYSYSTEM_H
#include <iostream>
#include <ostream>

#include "System.h"
#include "../Coordinator/Coordinator.h"
#include "../Components/Tag.h"

extern Coordinator gCoordinator;

class PrintEntitySystem : public System {
public:
    void PrintUpdate() {
        for (auto const& entity : mEntities) {
            auto& tag = gCoordinator.GetComponent<Tag>(entity);

            std::cout << "Entity Tag: " << tag.tagName << std::endl;
        }
    }
};

#endif //RIDICULOENGINE_PRINTENTITYSYSTEM_H
