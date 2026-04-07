#ifndef SWARM_RENDERER_SYSTEM_H
#define SWARM_RENDERER_SYSTEM_H

#include "game/core/player_state.h"
#include "raylib.h"

void SwarmRendererSystem_Draw(Vector2 playerPos, PlayerState* state);

#endif // SWARM_RENDERER_SYSTEM_H
