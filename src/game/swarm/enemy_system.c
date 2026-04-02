#include "game/swarm/enemy_system.h"
#include "framework_ecs/ecs_core.h"
#include "raymath.h"
#include <stdlib.h>
#include <math.h>

// --- Data Configuration ---
static EnemySpawnPreset g_SpawnPresets[] = {
    { SPAWN_SINGLE,   60.0f, {1, 1},   {500.0f, 700.0f}, ENEMY_BASIC },
    { SPAWN_LINE,     15.0f, {8, 12},  {700.0f, 1000.0f}, ENEMY_BASIC },
    { SPAWN_CLUSTER,  20.0f, {8, 20},  {600.0f, 800.0f},  ENEMY_BASIC },
    { SPAWN_CLUSTER,  10.0f, {20, 40}, {900.0f, 1200.0f}, ENEMY_FAST },  // A bigger, further cluster
    { SPAWN_SURROUND, 5.0f,  {30, 55}, {2000.0f, 2000.0f},  ENEMY_BASIC }
};

#define PRESET_COUNT (sizeof(g_SpawnPresets) / sizeof(g_SpawnPresets[0]))

static float g_SpawnTimer = 0.0f;
static float g_SpawnRate = 1.5f;

// Helper: Get random point at distance range from origin
static Vector2 GetRandomPointAtDistance(Vector2 origin, float minDist, float maxDist) {
    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
    float dist = (float)GetRandomValue((int)minDist, (int)maxDist);
    return (Vector2){ 
        origin.x + cosf(angle) * dist, 
        origin.y + sinf(angle) * dist 
    };
}

void EnemySystem_Init(Vector2 playerSpawnPos) {
    g_SpawnTimer = 0;
    // Immediate starting spawns using our new Preset 0 (Single)
    for (int i = 0; i < 10; i++) {
        EnemySystem_TriggerPreset(playerSpawnPos, g_SpawnPresets[0]);
    }
}

void EnemySystem_TriggerPreset(Vector2 playerPos, EnemySpawnPreset preset) {
    int count = GetRandomValue(preset.countRange.min, preset.countRange.max);
    Color enemyColor = RED;
    if (preset.enemyType == ENEMY_FAST) enemyColor = ORANGE;
    if (preset.enemyType == ENEMY_TANK) enemyColor = DARKPURPLE;

    switch (preset.pattern) {
        case SPAWN_SINGLE: {
            // Even if preset has count > 1, we treat SINGLE as its own logic usually
            for (int i = 0; i < count; i++) {
                Vector2 pos = GetRandomPointAtDistance(playerPos, preset.distRange.min, preset.distRange.max);
                ECS_SpawnEntity(pos, (Vector2){0,0}, enemyColor, 20.0f);
            }
        } break;

        case SPAWN_LINE: {
            Vector2 spawnCenter = GetRandomPointAtDistance(playerPos, preset.distRange.min, preset.distRange.max);
            Vector2 toPlayer = Vector2Normalize(Vector2Subtract(playerPos, spawnCenter));
            Vector2 side = { -toPlayer.y, toPlayer.x }; 
            
            float spacing = 30.0f;
            for (int i = 0; i < count; i++) {
                float offset = (i - count/2.0f) * spacing;
                Vector2 pos = Vector2Add(spawnCenter, Vector2Scale(side, offset));
                ECS_SpawnEntity(pos, (Vector2){0,0}, enemyColor, 20.0f);
            }
        } break;

        case SPAWN_CLUSTER: {
            Vector2 spawnCenter = GetRandomPointAtDistance(playerPos, preset.distRange.min, preset.distRange.max);
            for (int i = 0; i < count; i++) {
                Vector2 offset = { (float)GetRandomValue(-60, 60), (float)GetRandomValue(-60, 60) };
                ECS_SpawnEntity(Vector2Add(spawnCenter, offset), (Vector2){0,0}, enemyColor, 20.0f);
            }
        } break;

        case SPAWN_SURROUND: {
            float dist = preset.distRange.min; // Surround uses the min as the radius usually
            for (int i = 0; i < count; i++) {
                float angle = (i / (float)count) * PI * 2.0f;
                Vector2 pos = { playerPos.x + cosf(angle) * dist, playerPos.y + sinf(angle) * dist };
                ECS_SpawnEntity(pos, (Vector2){0,0}, enemyColor, 20.0f);
            }
        } break;
    }
}

void EnemySystem_Update(float deltaTime, Vector2 playerPos) {
    // 1. Weighted Random Spawning Logic
    g_SpawnTimer += deltaTime;
    if (g_SpawnTimer >= g_SpawnRate) {
        g_SpawnTimer = 0;
        
        float totalWeight = 0;
        for (int i = 0; i < PRESET_COUNT; i++) totalWeight += g_SpawnPresets[i].weight;
        
        float roll = (float)GetRandomValue(0, (int)(totalWeight * 100)) / 100.0f;
        float currentWeight = 0;
        
        for (int i = 0; i < PRESET_COUNT; i++) {
            currentWeight += g_SpawnPresets[i].weight;
            if (roll <= currentWeight) {
                EnemySystem_TriggerPreset(playerPos, g_SpawnPresets[i]);
                break;
            }
        }
    }

    // 2. Swarm AI (Keep same logic, just iterate active indices)
    float moveSpeed = 150.0f;
    float repulseRadius = 25.0f;
    float playerRadius = 35.0f;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (ecs_bIsActive[i]) {
            Vector2 desiredVelocity = {0, 0};
            
            // Move towards player
            Vector2 toPlayer = Vector2Subtract(playerPos, ecs_positions[i]);
            float distToPlayer = Vector2Length(toPlayer);
            
            if (distToPlayer > playerRadius) {
                Vector2 dir = Vector2Normalize(toPlayer);
                desiredVelocity = Vector2Scale(dir, moveSpeed);
            }
            
            // Separation
            Vector2 separation = {0, 0};
            for (int j = 0; j < MAX_ENTITIES; j++) {
                if (i != j && ecs_bIsActive[j]) {
                    Vector2 toOther = Vector2Subtract(ecs_positions[i], ecs_positions[j]);
                    float distSq = toOther.x * toOther.x + toOther.y * toOther.y;
                    if (distSq < repulseRadius * repulseRadius && distSq > 0.01f) {
                        float dist = sqrtf(distSq);
                        Vector2 repulse = Vector2Scale(toOther, 1.0f / dist);
                        float force = (repulseRadius - dist) / repulseRadius;
                        separation = Vector2Add(separation, Vector2Scale(repulse, moveSpeed * force * 2.5f));
                    }
                }
            }
            
            ecs_velocities[i] = Vector2Add(desiredVelocity, separation);
            ecs_positions[i].x += ecs_velocities[i].x * deltaTime;
            ecs_positions[i].y += ecs_velocities[i].y * deltaTime;
        }
    }
}
