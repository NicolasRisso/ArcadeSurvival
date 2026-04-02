#ifndef ECS_CORE_H
#define ECS_CORE_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_ENTITIES 1000

// Pure Data Arrays for Structure of Arrays architecture
extern Vector2 ecs_positions[MAX_ENTITIES];
extern Vector2 ecs_velocities[MAX_ENTITIES];
extern Color ecs_colors[MAX_ENTITIES];
extern float ecs_sizes[MAX_ENTITIES];
extern bool ecs_bIsActive[MAX_ENTITIES];

void ECS_Init(void);
int ECS_SpawnEntity(Vector2 pos, Vector2 vel, Color col, float size);
void ECS_DestroyEntity(int entityId);

#endif // ECS_CORE_H
