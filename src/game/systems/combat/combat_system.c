#include "game/systems/combat/combat_system.h"
#include "game/systems/combat/weapon_data.h"
#include "game/swarm/pickup_system.h"
#include "game/hud/popup_system.h"
#include "framework_ecs/ecs_core.h"
#include "game/audio/audio_manager.h"
#include "graphics/resource_manager.h"
#include "raymath.h"
#include <float.h>

extern float g_DoubleTroubleTimer;

void CombatSystem_Update(float deltaTime, PlayerState* state, Vector2 playerPos) {
    if (!state || state->health.bIsDead) return;

    for (int i = 0; i < state->weapons.activeWeaponCount; i++) {
        Weapon* w = &state->weapons.weapons[i];
        if (w->type == WEAPON_NONE) continue;

        const WeaponLevelStats* stats = GetWeaponStats(w->type, w->level);
        if (!stats) continue;

        float realFireRate = (g_DoubleTroubleTimer > 0.0f) ? stats->fireRate * 0.5f : stats->fireRate;
        realFireRate *= (1.0f / state->stats.attackSpeedMultiplier);

        int realDamage = (g_DoubleTroubleTimer > 0.0f) ? stats->damage * 2 : stats->damage;
        realDamage = (int)(realDamage * state->stats.damageMultiplier);

        float realSize = state->stats.sizeMultiplier;

        w->cooldownTimer -= deltaTime;

        if (w->cooldownTimer <= 0.0f) {
            bool fired = false;

            switch (w->type) {
                case WEAPON_FIREBALL_WAND: {
                    // Fires in 4 directions (multiple of 90 deg)
                    // Level 1-4: 1 per axis (4 total)
                    // Level 5-9: 2 per axis (8 total)
                    // Level 10-14: 3 per axis (12 total)
                    // Level 15: 4 per axis (16 total)
                    int perAxis = stats->projectileCount / 4;
                    for (int axis = 0; axis < 4; axis++) {
                        float angleBase = axis * 90.0f * (PI / 180.0f);
                        for (int j = 0; j < perAxis; j++) {
                            // Subtle offset for multiple projectiles
                            float angle = angleBase + (j - (perAxis-1)*0.5f) * 0.1f;
                            Vector2 dir = { cosf(angle), sinf(angle) };
                            ECS_SpawnProjectileEx(playerPos, Vector2Scale(dir, 300.0f), ORANGE, 8.0f * realSize, realDamage, stats->penetration, PROJ_FIREBALL, stats->specialValue, stats->damageCap);
                        }
                    }
                    fired = true;
                    // LifeSteal
                    if (state->stats.lifeSteal > 0.0f && realDamage > 0) {
                        float heal = realDamage * state->stats.lifeSteal * stats->projectileCount; // Approximated
                        HealthComponent_Heal(&state->health, (int)heal);
                    }
                    break;
                }

                case WEAPON_CRYSTAL_SHARD: {
                    // Fires at closest enemy
                    int closestIdx = -1;
                    float minDistSq = FLT_MAX;
                    for (int e = 0; e < MAX_ENEMIES; e++) {
                        if (enemy_bIsActive[e]) {
                            float d2 = Vector2DistanceSqr(playerPos, enemy_positions[e]);
                            if (d2 < minDistSq) { minDistSq = d2; closestIdx = e; }
                        }
                    }

                    if (closestIdx != -1) {
                        Vector2 target = enemy_positions[closestIdx];
                        Vector2 baseDir = Vector2Normalize(Vector2Subtract(target, playerPos));
                        for (int j = 0; j < stats->projectileCount; j++) {
                            float spread = (j - (stats->projectileCount-1)*0.5f) * 0.2f;
                            Vector2 dir = Vector2Rotate(baseDir, spread);
                            ECS_SpawnProjectileEx(playerPos, Vector2Scale(dir, 450.0f), BLUE, 5.0f * realSize, realDamage, stats->penetration, PROJ_NORMAL, 0.0f, stats->damageCap);
                        }
                        fired = true;
                        // LifeSteal
                        if (state->stats.lifeSteal > 0.0f) {
                            HealthComponent_Heal(&state->health, (int)(realDamage * state->stats.lifeSteal * stats->projectileCount));
                        }
                    }
                    break;
                }

                case WEAPON_DEATH_AURA: {
                    // Death Aura deal damage every tick (stats->fireRate)
                    // We can just use the existing loop and apply damage to all enemies in range
                    for (int e = 0; e < MAX_ENEMIES; e++) {
                        if (enemy_bIsActive[e]) {
                            if (Vector2Distance(playerPos, enemy_positions[e]) < stats->range * realSize) {
                                enemy_healths[e] -= realDamage;
                                enemy_damageFlashes[e] = 0.1f;
                                AudioManager_PlaySfxThrottled(SND_ENEMY_HIT, 3);
                                PopupSystem_Add(enemy_positions[e], realDamage);
                                if (state->stats.lifeSteal > 0.0f) {
                                    HealthComponent_Heal(&state->health, (int)(realDamage * state->stats.lifeSteal));
                                }
                                if (enemy_healths[e] <= 0) {
                                    int xpValue = 10;
                                    if (enemy_types[e] == ENEMY_FAST) xpValue = 25;
                                    else if (enemy_types[e] == ENEMY_TANK) xpValue = 100;

                                    int thirtySecPeriods = (int)(state->gameTime / 30.0f);
                                    float xpMultiplier = 1.0f + (thirtySecPeriods * 0.25f);
                                    xpValue = (int)(xpValue * xpMultiplier);

                                    PickupSystem_RollLoot(enemy_positions[e], xpValue);
                                    ECS_DestroyEnemy(e);
                                }
                            }
                        }
                    }
                    fired = true;
                    break;
                }

                case WEAPON_BOMB_SHOES: {
                    // Drops a bomb at feet. We divide range by 4 because SpawnExplosion multiplies it by 4
                    float bombSize = (stats->range / 4.0f) * realSize;
                    ECS_SpawnProjectileEx(playerPos, (Vector2){0,0}, DARKGRAY, bombSize, realDamage, 1, PROJ_BOMB, stats->specialValue, stats->damageCap);
                    fired = true;
                    if (state->stats.lifeSteal > 0.0f) {
                        HealthComponent_Heal(&state->health, (int)(realDamage * state->stats.lifeSteal));
                    }
                    break;
                }

                case WEAPON_NATURE_SPIKES: {
                    // Target a random enemy in range
                    int activeEnemies[MAX_ENEMIES];
                    int count = 0;
                    for (int e = 0; e < MAX_ENEMIES; e++) {
                        if (enemy_bIsActive[e]) activeEnemies[count++] = e;
                    }

                    if (count > 0) {
                        int targetIdx = activeEnemies[GetRandomValue(0, count - 1)];
                        Vector2 targetPos = enemy_positions[targetIdx];
                        // Spawn a spike at feet. Timer is 2s (or 3s at lvl 10+)
                        float lifeTime = (w->level >= 10) ? 3.0f : 2.0f;
                        ECS_SpawnProjectileEx(targetPos, (Vector2){0,0}, GREEN, 15.0f * realSize, realDamage, 999, PROJ_SPIKE, lifeTime, stats->damageCap);
                        fired = true;
                        if (state->stats.lifeSteal > 0.0f) {
                            HealthComponent_Heal(&state->health, (int)(realDamage * state->stats.lifeSteal));
                        }
                    }
                    break;
                }

                default: break;
            }

            if (fired) {
                w->cooldownTimer = realFireRate;
            }
        }
    }
}
