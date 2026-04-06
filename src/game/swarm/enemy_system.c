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
    { SPAWN_SINGLE,   5.0f,  {1, 3},   {800.0f, 900.0f},  ENEMY_TANK }, // Tank spawns infrequently
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
    float baseSpeed = 150.0f;
    int health = 15;
    int damage = 7;
    float size = 20.0f;
    
    if (preset.enemyType == ENEMY_FAST) {
        enemyColor = ORANGE;
        baseSpeed = 250.0f;
        health = 5;
        damage = 3;
        size = 15.0f;
    }
    if (preset.enemyType == ENEMY_TANK) {
        enemyColor = DARKPURPLE;
        baseSpeed = 90.0f;
        health = 80;
        damage = 15;
        size = 35.0f;
    }

    switch (preset.pattern) {
        case SPAWN_SINGLE: {
            for (int i = 0; i < count; i++) {
                Vector2 pos = GetRandomPointAtDistance(playerPos, preset.distRange.min, preset.distRange.max);
                float speed = baseSpeed + (float)GetRandomValue(-20, 20);
                ECS_SpawnEnemy(pos, enemyColor, size, health, speed, damage, preset.enemyType);
            }
        } break;

        case SPAWN_LINE: {
            Vector2 spawnCenter = GetRandomPointAtDistance(playerPos, preset.distRange.min, preset.distRange.max);
            Vector2 toPlayer = Vector2Normalize(Vector2Subtract(playerPos, spawnCenter));
            Vector2 side = { -toPlayer.y, toPlayer.x }; 
            
            float spacing = size * 1.5f;
            for (int i = 0; i < count; i++) {
                float offset = (i - count/2.0f) * spacing;
                Vector2 pos = Vector2Add(spawnCenter, Vector2Scale(side, offset));
                float speed = baseSpeed + (float)GetRandomValue(-20, 20);
                ECS_SpawnEnemy(pos, enemyColor, size, health, speed, damage, preset.enemyType);
            }
        } break;

        case SPAWN_CLUSTER: {
            Vector2 spawnCenter = GetRandomPointAtDistance(playerPos, preset.distRange.min, preset.distRange.max);
            for (int i = 0; i < count; i++) {
                Vector2 offset = { (float)GetRandomValue(-60, 60), (float)GetRandomValue(-60, 60) };
                float speed = baseSpeed + (float)GetRandomValue(-20, 20);
                ECS_SpawnEnemy(Vector2Add(spawnCenter, offset), enemyColor, size, health, speed, damage, preset.enemyType);
            }
        } break;

        case SPAWN_SURROUND: {
            float dist = preset.distRange.min; // Surround uses the min as the radius usually
            for (int i = 0; i < count; i++) {
                float angle = (i / (float)count) * PI * 2.0f;
                Vector2 pos = { playerPos.x + cosf(angle) * dist, playerPos.y + sinf(angle) * dist };
                float speed = baseSpeed + (float)GetRandomValue(-20, 20);
                ECS_SpawnEnemy(pos, enemyColor, size, health, speed, damage, preset.enemyType);
            }
        } break;
    }
}

void EnemySystem_Update(float deltaTime, Vector2 playerPos, PlayerState* playerState) {
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

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_bIsActive[i]) {
            extern float g_TimeFreezeTimer;
            // Update timers (these should always update even if time is frozen 
            // to allow damage flashes to wear off)
            if (enemy_attackTimers[i] > 0.0f) {
                enemy_attackTimers[i] -= deltaTime;
            }
            if (enemy_damageFlashes[i] > 0.0f) {
                enemy_damageFlashes[i] -= deltaTime;
            }

            // If time is frozen, skip movement and attacking
            if (g_TimeFreezeTimer > 0.0f) {
                enemy_velocities[i] = (Vector2){0, 0};
                continue;
            }

            float repulseRadius = 25.0f;
            float playerRadius = 35.0f;

            Vector2 desiredVelocity = {0, 0};
            float moveSpeed = enemy_maxSpeeds[i];
            
            // Move towards player
            Vector2 toPlayer = Vector2Subtract(playerPos, enemy_positions[i]);
            float distToPlayer = Vector2Length(toPlayer);
            
            float hitThreshold = playerRadius + enemy_sizes[i]/2.0f;
            
            // If overlapping player and can attack
            if (distToPlayer <= hitThreshold) {
                if (enemy_attackTimers[i] <= 0.0f) {
                    // Deal damage
                    PlayerState_TakeDamage(playerState, enemy_damages[i]);
                    enemy_attackTimers[i] = 0.5f; // Freeze for 0.5s
                }
            }
            
            // Only move if not recovering from attack
            if (enemy_attackTimers[i] <= 0.0f) {
                if (distToPlayer > playerRadius) {
                    Vector2 dir = Vector2Scale(toPlayer, 1.0f / distToPlayer);
                    desiredVelocity = Vector2Scale(dir, moveSpeed);
                }
                
                // Separation
                Vector2 separation = {0, 0};
                for (int j = 0; j < MAX_ENEMIES; j++) {
                    if (i != j && enemy_bIsActive[j]) {
                        Vector2 toOther = Vector2Subtract(enemy_positions[i], enemy_positions[j]);
                        float distSq = toOther.x * toOther.x + toOther.y * toOther.y;
                        if (distSq < repulseRadius * repulseRadius && distSq > 0.01f) {
                            float dist = sqrtf(distSq);
                            Vector2 repulse = Vector2Scale(toOther, 1.0f / dist);
                            float force = (repulseRadius - dist) / repulseRadius;
                            separation = Vector2Add(separation, Vector2Scale(repulse, moveSpeed * force * 2.5f));
                        }
                    }
                }
                
                enemy_velocities[i] = Vector2Add(desiredVelocity, separation);
                enemy_positions[i].x += enemy_velocities[i].x * deltaTime;
                enemy_positions[i].y += enemy_velocities[i].y * deltaTime;
            } else {
                enemy_velocities[i] = (Vector2){0, 0};
            }
        }
    }
}
