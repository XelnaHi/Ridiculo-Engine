//
// Created by Teo on 2026-11-26.
//

#ifndef RIDICULOENGINE_SYSTEM_H
#define RIDICULOENGINE_SYSTEM_H
#include <set>

#include "../Entity/Entity.h"

class System {
public:
    std::set<Entity> mEntities;
};

#endif //RIDICULOENGINE_SYSTEM_H
