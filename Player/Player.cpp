//
// Created by teo95 on 2026-11-17.
//

#include "Player.h"

#include <iostream>

#include "../Camera/Camera.h"
#include "../InputSystem/InputSystem.h"

Player::Player() : camera(glm::vec3(0.0f, 0.0f, 3.0f)) {
}

void Player::update(const PlayerInput &input, float deltaTime) {
    camera.Move(input.moveZ, deltaTime);
    camera.Rotate(input.rotateYaw, input.rotatePitch, deltaTime);
}

void Player::TeleportTo(const glm::vec3 &position) {
    camera.SetPosition(position);
}
