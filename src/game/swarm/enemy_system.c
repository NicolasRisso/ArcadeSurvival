#include "game/swarm/enemy_system.h"
#include "framework_ecs/ecs_core.h"
#include "raymath.h"
#include <stdlib.h>

void EnemySystem_Init(Vector2 playerSpawnPos) {
    // Spawn 5 test enemies around the player
    for (int i = 0; i < 5; i++) {
        Vector2 spawnOffset = {
            (float)(GetRandomValue(-200, 200)),
            (float)(GetRandomValue(-200, 200))
        };
        Vector2 enemyPos = Vector2Add(playerSpawnPos, spawnOffset);
        
        // Give them a random velocity for testing
        Vector2 enemyVel = {
            (float)(GetRandomValue(-50, 50)),
            (float)(GetRandomValue(-50, 50))
        };
        
        ECS_SpawnEntity(enemyPos, enemyVel, RED, 20.0f);
    }
}

void EnemySystem_Update(float deltaTime) {
    // Update logic for active enemies
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (ecs_bIsActive[i]) {
            ecs_positions[i].x += ecs_velocities[i].x * deltaTime;
            ecs_positions[i].y += ecs_velocities[i].y * deltaTime;
            
            // Screen bounce logic just for testing Act 4 visually!
            if (ecs_positions[i].x < 0 || ecs_positions[i].x > 800) ecs_velocities[i].x *= -1;
            if (ecs_positions[i].y < 0 || ecs_positions[i].y > 600) ecs_velocities[i].y *= -1;
        }
    }
}
