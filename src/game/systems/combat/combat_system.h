#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "game/core/player_state.h"
#include "raylib.h"

void CombatSystem_Update(float deltaTime, PlayerState* state, Vector2 playerPos);

#endif // COMBAT_SYSTEM_H
