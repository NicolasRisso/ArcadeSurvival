#include "framework_ecs/ecs_core.h"

// Concrete allocation of massive data arrays
Vector2 ecs_positions[MAX_ENTITIES];
Vector2 ecs_velocities[MAX_ENTITIES];
Color ecs_colors[MAX_ENTITIES];
float ecs_sizes[MAX_ENTITIES];
bool ecs_bIsActive[MAX_ENTITIES];

void ECS_Init(void) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        ecs_bIsActive[i] = false;
        ecs_sizes[i] = 10.0f;
    }
}

int ECS_SpawnEntity(Vector2 pos, Vector2 vel, Color col, float size) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!ecs_bIsActive[i]) {
            ecs_positions[i] = pos;
            ecs_velocities[i] = vel;
            ecs_colors[i] = col;
            ecs_sizes[i] = size;
            ecs_bIsActive[i] = true;
            return i;
        }
    }
    return -1; // No space left
}

void ECS_DestroyEntity(int entityId) {
    if (entityId >= 0 && entityId < MAX_ENTITIES) {
        ecs_bIsActive[entityId] = false;
    }
}
