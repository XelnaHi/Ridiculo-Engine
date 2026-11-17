//
// Created by teo95 on 2026-11-17.
//

#ifndef RIDICULOENGINE_PLAYER_H
#define RIDICULOENGINE_PLAYER_H


class Controller;

class Player {
public:
    Player(Controller *controller);

    Controller* controller;
};


#endif //RIDICULOENGINE_PLAYER_H
