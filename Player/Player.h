//
// Created by teo95 on 2026-11-17.
//

#ifndef RIDICULOENGINE_PLAYER_H
#define RIDICULOENGINE_PLAYER_H
#include "../Camera/Camera.h"

struct PlayerInput;
class Controller;

class Player {
public:
    explicit Player();
    void update(const PlayerInput &input, float deltaTime);

    void TeleportTo(const glm::vec3 &position);

    // Storing a member by value means that when creating a player, the player object reserves memory for
    // the sizeof(Camera). We use a member initializer list to initialize the camera values (world position, so far)
    // as part of the Player constructor.
    Camera camera;
};


#endif //RIDICULOENGINE_PLAYER_H
