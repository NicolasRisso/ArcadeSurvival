#include "game/swarm/projectile_system.h"
#include "game/swarm/pickup_system.h"
#include "game/hud/popup_system.h"
#include "framework_ecs/ecs_core.h"
#include "raymath.h"
#include <string.h>
#include "game/audio/audio_manager.h"
#include "graphics/resource_manager.h"

static void SpawnExplosion(Vector2 pos, float radius, int damage) {
    AudioManager_PlaySfx(SND_EXPLOSION);
    ECS_SpawnProjectileEx(pos, (Vector2){0,0}, YELLOW, radius, damage, 999, PROJ_EXPLOSION, 0.1f, 2147483647);
}

void ProjectileSystem_Update(float deltaTime, PlayerState* state) {
    if (!state) return;
    // XP scales by 20% every 30 seconds
    int thirtySecPeriods = (int)(state->gameTime / 30.0f);
    float xpMultiplier = 1.0f + (thirtySecPeriods * 0.20f);

    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectile_bIsActive[i]) continue;

        // 1. Update Timer
        if (projectile_timers[i] > 0.0f) {
            projectile_timers[i] -= deltaTime;
            if (projectile_timers[i] <= 0.0f) {
                // Timeout logic
                if (projectile_types[i] == PROJ_FIREBALL || projectile_types[i] == PROJ_BOMB) {
                    SpawnExplosion(projectile_positions[i], projectile_sizes[i] * 4.0f, projectile_damage[i] / 2);
                }
                ECS_DestroyProjectile(i);
                continue;
            }
        }

        // 2. Move Projectile
        projectile_positions[i].x += projectile_velocities[i].x * deltaTime;
        projectile_positions[i].y += projectile_velocities[i].y * deltaTime;

        // 3. Special Ticking Logic
        if (projectile_types[i] == PROJ_SPIKE) {
            projectile_specialTimers[i] -= deltaTime;
            if (projectile_specialTimers[i] <= 0.0f) {
                // Reset hit mask to allow re-hitting enemies next tick
                memset(projectile_hitMasks[i], 0, sizeof(projectile_hitMasks[i]));
                projectile_specialTimers[i] = 0.2f; // Default tick rate, can be adjusted
            }
        }

        // 4. Collision Detection
        float hitRadiusSq = (20.0f + projectile_sizes[i]) * (20.0f + projectile_sizes[i]);
        
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemy_bIsActive[j]) continue;
            
            float distSq = Vector2DistanceSqr(enemy_positions[j], projectile_positions[i]);

            if (distSq < hitRadiusSq) {
                // Check hit mask
                int byteIdx = j / 8;
                int bitIdx = j % 8;
                if (projectile_hitMasks[i][byteIdx] & (1 << bitIdx)) continue;

                // Mark as hit
                projectile_hitMasks[i][byteIdx] |= (1 << bitIdx);

                // Apply damage
                if (projectile_types[i] != PROJ_BOMB) {
                    enemy_healths[j] -= projectile_damage[i];
                    enemy_damageFlashes[j] = 0.1f;
                    AudioManager_PlaySfxThrottled(SND_ENEMY_HIT, 3);
                    PopupSystem_Add(enemy_positions[j], projectile_damage[i]);
                    projectile_damageDealt[i] += projectile_damage[i];
                }

                if (projectile_damageDealt[i] >= projectile_damageCaps[i]) {
                    ECS_DestroyProjectile(i);
                    break;
                }

                if (enemy_healths[j] <= 0) {
                    int xpValue = 10;
                    if (enemy_types[j] == ENEMY_FAST) xpValue = 25;
                    else if (enemy_types[j] == ENEMY_TANK) xpValue = 100;
                    
                    // Apply time scaling
                    xpValue = (int)(xpValue * xpMultiplier);
                    
                    PickupSystem_RollLoot(enemy_positions[j], xpValue);
                    ECS_DestroyEnemy(j);
                }

                // Interaction logic
                if (projectile_types[i] == PROJ_NORMAL || projectile_types[i] == PROJ_FIREBALL) {
                    projectile_penetrations[i]--;
                    if (projectile_penetrations[i] <= 0) {
                        if (projectile_types[i] == PROJ_FIREBALL) {
                            SpawnExplosion(projectile_positions[i], projectile_sizes[i] * 4.0f, projectile_damage[i] / 2);
                        }
                        ECS_DestroyProjectile(i);
                        break;
                    }
                }
            }
        }
    }
}
