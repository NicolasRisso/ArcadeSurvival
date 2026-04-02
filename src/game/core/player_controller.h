#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "raylib.h"

typedef struct {
    Vector2 moveInput;
} PlayerController;

void PlayerController_Init(PlayerController* controller);
void PlayerController_Update(PlayerController* controller);

#endif // PLAYER_CONTROLLER_H
