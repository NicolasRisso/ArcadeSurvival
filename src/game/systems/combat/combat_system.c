#include "game/systems/combat/combat_system.h"
#include "framework_ecs/ecs_core.h"
#include "raymath.h"
#include <float.h>

extern float g_DoubleTroubleTimer;

void CombatSystem_Update(float deltaTime, PlayerState* state, Vector2 playerPos) {
    if (!state || state->health.bIsDead) return;

    for (int i = 0; i < state->weapons.activeWeaponCount; i++) {
        Weapon* w = &state->weapons.weapons[i];
        
        if (w->type == WEAPON_NONE) continue;
        
        float realFireRate = (g_DoubleTroubleTimer > 0.0f) ? w->fireRate * 0.5f : w->fireRate;
        int realDamage = (g_DoubleTroubleTimer > 0.0f) ? w->damage * 2 : w->damage;
        
        w->cooldownTimer -= deltaTime;
        
        if (w->cooldownTimer <= 0.0f) {
            // Find closest enemy
            int closestEnemyIdx = -1;
            float closestDistSq = FLT_MAX;
            
            for (int e = 0; e < MAX_ENEMIES; e++) {
                if (enemy_bIsActive[e]) {
                    Vector2 toEnemy = Vector2Subtract(enemy_positions[e], playerPos);
                    float distSq = toEnemy.x * toEnemy.x + toEnemy.y * toEnemy.y;
                    if (distSq < closestDistSq) {
                        closestDistSq = distSq;
                        closestEnemyIdx = e;
                    }
                }
            }
            
            if (closestEnemyIdx != -1) {
                // Fire weapon
                // Arrow shoots straight towards the enemy
                Vector2 targetPos = enemy_positions[closestEnemyIdx];
                Vector2 dir = Vector2Normalize(Vector2Subtract(targetPos, playerPos));
                float projSpeed = 400.0f;
                Vector2 vel = Vector2Scale(dir, projSpeed);
                
                ECS_SpawnProjectile(
                    playerPos, 
                    vel, 
                    BLUE, 
                    6.0f, 
                    realDamage, 
                    w->penetration
                );
                
                w->cooldownTimer = realFireRate; // Reset cooldown
            }
        }
    }
}
