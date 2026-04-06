#include "game/swarm/pickup_system.h"
#include "framework_ecs/ecs_core.h"
#include "raymath.h"

float g_TimeFreezeTimer = 0.0f;
float g_DoubleTroubleTimer = 0.0f;

static void TriggerMagnet(void) {
    for (int i = 0; i < MAX_PICKUPS; i++) {
        if (pickup_bIsActive[i]) {
            pickup_bIsMagnetized[i] = true;
        }
    }
}

static void TriggerNuke(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_bIsActive[i]) {
            // Kill instantly 
            ECS_DestroyEnemy(i);
        }
    }
}

void PickupSystem_Update(float deltaTime, PlayerState* state, Vector2 playerPos) {
    // Update global timers
    if (g_TimeFreezeTimer > 0.0f) g_TimeFreezeTimer -= deltaTime;
    if (g_DoubleTroubleTimer > 0.0f) g_DoubleTroubleTimer -= deltaTime;

    float pickupRadiusSq = 30.0f * 30.0f;
    float magnetZoneSq = 200.0f * 200.0f;
    float maxMagnetSpeed = 600.0f;
    float magnetAccel = 1200.0f; // Speed gained per second

    for (int i = 0; i < MAX_PICKUPS; i++) {
        if (!pickup_bIsActive[i]) continue;

        Vector2 toPlayer = Vector2Subtract(playerPos, pickup_positions[i]);
        float distSq = toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y;

        // Check if inside magnetize zone or already magnetized
        if (pickup_bIsMagnetized[i] || distSq < magnetZoneSq) {
            pickup_bIsMagnetized[i] = true;
            
            // Accelerate towards player
            pickup_magnetizeSpeeds[i] += magnetAccel * deltaTime;
            if (pickup_magnetizeSpeeds[i] > maxMagnetSpeed) {
                pickup_magnetizeSpeeds[i] = maxMagnetSpeed;
            }
            
            // Move item
            if (distSq > 0.1f) {
                Vector2 dir = Vector2Scale(toPlayer, 1.0f / sqrtf(distSq));
                pickup_positions[i].x += dir.x * pickup_magnetizeSpeeds[i] * deltaTime;
                pickup_positions[i].y += dir.y * pickup_magnetizeSpeeds[i] * deltaTime;
                
                // Re-evaluate distSq for pickup
                toPlayer = Vector2Subtract(playerPos, pickup_positions[i]);
                distSq = toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y;
            }
        }

        // Check pickup radius
        if (distSq <= pickupRadiusSq) {
            switch (pickup_types[i]) {
                case PICKUP_XP_GEM:
                    PlayerState_AddExperience(state, 10);
                    break;
                case PICKUP_NUKE:
                    TriggerNuke();
                    break;
                case PICKUP_TIME_FREEZE:
                    g_TimeFreezeTimer = 15.0f;
                    break;
                case PICKUP_DOUBLE_TROUBLE:
                    g_DoubleTroubleTimer = 15.0f;
                    break;
                case PICKUP_MAGNET:
                    TriggerMagnet();
                    break;
            }
            ECS_DestroyPickup(i);
        }
    }
}

void PickupSystem_DrawBackground(void) {
    for (int i = 0; i < MAX_PICKUPS; i++) {
        if (pickup_bIsActive[i] && !pickup_bIsMagnetized[i]) {
            Color col = WHITE;
            float size = 8.0f;

            switch (pickup_types[i]) {
                case PICKUP_XP_GEM:         col = BLUE; size = 6.0f; break;
                case PICKUP_NUKE:           col = RED; size = 12.0f; break;
                case PICKUP_TIME_FREEZE:    col = SKYBLUE; size = 12.0f; break;
                case PICKUP_DOUBLE_TROUBLE: col = ORANGE; size = 12.0f; break;
                case PICKUP_MAGNET:         col = MAGENTA; size = 12.0f; break;
            }

            DrawRectangleV((Vector2){ pickup_positions[i].x - size/2, pickup_positions[i].y - size/2 }, 
                           (Vector2){ size, size }, col);
        }
    }
}

void PickupSystem_DrawForeground(void) {
    for (int i = 0; i < MAX_PICKUPS; i++) {
        if (pickup_bIsActive[i] && pickup_bIsMagnetized[i]) {
            Color col = WHITE;
            float size = 8.0f;

            switch (pickup_types[i]) {
                case PICKUP_XP_GEM:         col = BLUE; size = 6.0f; break;
                case PICKUP_NUKE:           col = RED; size = 12.0f; break;
                case PICKUP_TIME_FREEZE:    col = SKYBLUE; size = 12.0f; break;
                case PICKUP_DOUBLE_TROUBLE: col = ORANGE; size = 12.0f; break;
                case PICKUP_MAGNET:         col = MAGENTA; size = 12.0f; break;
            }

            DrawRectangleV((Vector2){ pickup_positions[i].x - size/2, pickup_positions[i].y - size/2 }, 
                           (Vector2){ size, size }, col);
        }
    }
}