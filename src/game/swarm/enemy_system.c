#include "game/swarm/enemy_system.h"
#include "framework_ecs/ecs_core.h"
#include "raymath.h"
#include <stdlib.h>

void EnemySystem_Init(Vector2 playerSpawnPos) {
    // Spawn 5 test enemies around the player
    for (int i = 0; i < 5; i++) {
        Vector2 spawnOffset = {
            (float)(GetRandomValue(-400, 400)),
            (float)(GetRandomValue(-400, 400))
        };
        Vector2 enemyPos = Vector2Add(playerSpawnPos, spawnOffset);
        
        Vector2 enemyVel = {0, 0}; // Start still
        
        ECS_SpawnEntity(enemyPos, enemyVel, RED, 20.0f);
    }
}

void EnemySystem_Update(float deltaTime, Vector2 playerPos) {
    float moveSpeed = 150.0f;
    float repulseRadius = 25.0f;
    float playerRadius = 35.0f;

    // Update logic for active enemies
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (ecs_bIsActive[i]) {
            Vector2 desiredVelocity = {0, 0};
            
            // 1. Move towards player
            Vector2 toPlayer = Vector2Subtract(playerPos, ecs_positions[i]);
            float distToPlayer = Vector2Length(toPlayer);
            
            if (distToPlayer > playerRadius) {
                Vector2 dir = Vector2Normalize(toPlayer);
                desiredVelocity = Vector2Scale(dir, moveSpeed);
            }
            
            // 2. Separate from other enemies
            Vector2 separation = {0, 0};
            for (int j = 0; j < MAX_ENTITIES; j++) {
                if (i != j && ecs_bIsActive[j]) {
                    Vector2 toOther = Vector2Subtract(ecs_positions[i], ecs_positions[j]);
                    float distToOther = Vector2Length(toOther);
                    if (distToOther < repulseRadius && distToOther > 0.1f) {
                        Vector2 repulse = Vector2Normalize(toOther);
                        // The closer they are, the stronger the push
                        float repulseForce = (repulseRadius - distToOther) / repulseRadius;
                        separation = Vector2Add(separation, Vector2Scale(repulse, moveSpeed * repulseForce * 2.0f));
                    }
                }
            }
            
            ecs_velocities[i] = Vector2Add(desiredVelocity, separation);

            // Apply integration
            ecs_positions[i].x += ecs_velocities[i].x * deltaTime;
            ecs_positions[i].y += ecs_velocities[i].y * deltaTime;
        }
    }
}
