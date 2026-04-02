#include "game/swarm/swarm_renderer_system.h"
#include "framework_ecs/ecs_core.h"
#include "raylib.h"

void SwarmRendererSystem_Draw(void) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (ecs_bIsActive[i]) {
            DrawRectangle(
                (int)ecs_positions[i].x, 
                (int)ecs_positions[i].y, 
                (int)ecs_sizes[i], 
                (int)ecs_sizes[i], 
                ecs_colors[i]
            );
        }
    }
}
