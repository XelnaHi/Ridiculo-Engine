//
// Created by Teo on 2026-11-24.
//

#ifndef RIDICULOENGINE_TRANSFORM_H
#define RIDICULOENGINE_TRANSFORM_H
#include "glm/glm.hpp"

struct Transform {
    glm::vec3 Position;
    glm::vec3 Scale;


    /* Leaving rotation out of ECS conversion because I feel I might need to think harder here
     */
    // glm::vec3 Rotation;
};
#endif //RIDICULOENGINE_TRANSFORM_H
