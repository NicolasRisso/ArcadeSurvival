#include "game/swarm/swarm_renderer_system.h"
#include "game/core/player_state.h"
#include "game/systems/combat/combat_system.h"
#include "game/systems/combat/weapon_data.h"
#include "framework_ecs/ecs_core.h"
#include "raylib.h"
#include <stdio.h>

void SwarmRendererSystem_Draw(Vector2 playerPos, PlayerState* state) {
    // 1. Draw Death Aura (Background)
    if (state) {
        for (int i = 0; i < state->weapons.activeWeaponCount; i++) {
            if (state->weapons.weapons[i].type == WEAPON_DEATH_AURA) {
                const WeaponLevelStats* stats = GetWeaponStats(WEAPON_DEATH_AURA, state->weapons.weapons[i].level);
                if (stats) {
                    DrawCircleV(playerPos, stats->range, (Color){ 0, 0, 0, 80 }); // Black transparent
                }
            }
        }
    }

    // 2. Draw Enemies
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_bIsActive[i]) {
            extern float enemy_damageFlashes[];
            Color finalColor = (enemy_damageFlashes[i] > 0.0f) ? WHITE : enemy_colors[i];
            
            DrawRectangle(
                (int)enemy_positions[i].x - (int)enemy_sizes[i]/2, 
                (int)enemy_positions[i].y - (int)enemy_sizes[i]/2, 
                (int)enemy_sizes[i], 
                (int)enemy_sizes[i], 
                finalColor
            );
        }
    }

    // 3. Draw Projectiles
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectile_bIsActive[i]) {
            switch (projectile_types[i]) {
                case PROJ_FIREBALL:
                    DrawCircleV(projectile_positions[i], projectile_sizes[i], ORANGE);
                    DrawCircleLinesV(projectile_positions[i], projectile_sizes[i], RED);
                    break;
                case PROJ_BOMB:
                    DrawCircleV(projectile_positions[i], projectile_sizes[i], DARKGRAY);
                    DrawCircleV(projectile_positions[i], projectile_sizes[i] * 0.4f, RED); // Fuse?
                    break;
                case PROJ_SPIKE:
                    DrawPoly(projectile_positions[i], 3, projectile_sizes[i], 0, GREEN);
                    break;
                case PROJ_EXPLOSION:
                    DrawCircleGradient((int)projectile_positions[i].x, (int)projectile_positions[i].y, projectile_sizes[i], (Color){255, 200, 0, 150}, (Color){255, 0, 0, 0});
                    break;
                default:
                    DrawRectangle(
                        (int)projectile_positions[i].x - (int)projectile_sizes[i]/2, 
                        (int)projectile_positions[i].y - (int)projectile_sizes[i]/2, 
                        (int)projectile_sizes[i], 
                        (int)projectile_sizes[i], 
                        projectile_colors[i]
                    );
                    break;
            }
        }
    }
}
