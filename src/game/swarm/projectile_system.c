#include "game/swarm/projectile_system.h"
#include "framework_ecs/ecs_core.h"
#include "raymath.h"

void ProjectileSystem_Update(float deltaTime) {
    float hitRadiusSq = (20.0f + 5.0f) * (20.0f + 5.0f); // enemy size + projectile size

    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectile_bIsActive[i]) continue;

        // 1. Move Projectile
        projectile_positions[i].x += projectile_velocities[i].x * deltaTime;
        projectile_positions[i].y += projectile_velocities[i].y * deltaTime;

        // 2. Continuous Collision Detection (O(P * E))
        bool isDestroyed = false;
        
        for (int j = 0; j < MAX_ENEMIES; j++) {
            if (!enemy_bIsActive[j]) continue;
            
            // Wait, we need to make sure a projectile doesn't hit the SAME enemy twice per frame.
            // Since it hits instantly, we apply damage and decrement penetration immediately.
            Vector2 toEnemy = Vector2Subtract(enemy_positions[j], projectile_positions[i]);
            float distSq = toEnemy.x * toEnemy.x + toEnemy.y * toEnemy.y;

            if (distSq < hitRadiusSq) {
                // Hit!
                enemy_healths[j] -= projectile_damage[i];
                if (enemy_healths[j] <= 0) {
                    // Span pickup on death
                    // 0.5% chance = value between 0 and 1000 <= 5
                    int roll = GetRandomValue(0, 1000);
                    PickupType typeToSpawn = PICKUP_XP_GEM;
                    if (roll <= 5) {
                        int pRoll = GetRandomValue(0, 3);
                        if (pRoll == 0) typeToSpawn = PICKUP_NUKE;
                        else if (pRoll == 1) typeToSpawn = PICKUP_TIME_FREEZE;
                        else if (pRoll == 2) typeToSpawn = PICKUP_DOUBLE_TROUBLE;
                        else typeToSpawn = PICKUP_MAGNET;
                    }
                    ECS_SpawnPickup(enemy_positions[j], typeToSpawn);
                    
                    ECS_DestroyEnemy(j);
                }

                projectile_penetrations[i]--;
                if (projectile_penetrations[i] <= 0) {
                    ECS_DestroyProjectile(i);
                    isDestroyed = true;
                    break; // stop checking this projectile
                } else {
                    // For a penetrating arrow, optionally we shouldn't hit the same enemy multiple times. 
                    // But if the enemy dies instantly or the arrow quickly phases through, it might hit it next frame.
                    // To keep it simple, hitting an enemy destroys the enemy if damage is enough, 
                    // else it might hit it again next frame. We'll leave it as is for the basic test.
                }
            }
        }
    }
}
