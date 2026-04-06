#ifndef PICKUP_SYSTEM_H
#define PICKUP_SYSTEM_H

#include "raylib.h"
#include "game/core/player_state.h"

// Expose these global timers for power-up effects
extern float g_TimeFreezeTimer;
extern float g_DoubleTroubleTimer;

void PickupSystem_Update(float deltaTime, PlayerState* state, Vector2 playerPos);
void PickupSystem_Draw(void); // To render the pickups

#endif // PICKUP_SYSTEM_H
