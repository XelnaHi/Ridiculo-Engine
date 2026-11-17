//
// Created by teo95 on 2026-11-17.
//

#include "Player.h"

#include <iostream>

#include "../Camera/Camera.h"
#include "../PlayerInput/PlayerInput.h"

Player::Player(Camera *camera) : camera(camera) {
}

void Player::update(const PlayerInput &input) {
    camera->Move(input.moveZ);

    camera->Rotate(input.rotateYaw);
}
