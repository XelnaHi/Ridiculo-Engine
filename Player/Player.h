//
// Created by teo95 on 2026-11-17.
//

#ifndef RIDICULOENGINE_PLAYER_H
#define RIDICULOENGINE_PLAYER_H


struct PlayerInput;
class Camera;
class Controller;

class Player {
public:
    Player(Camera *camera);
    void update(const PlayerInput& input);

    Camera* camera;
};


#endif //RIDICULOENGINE_PLAYER_H
