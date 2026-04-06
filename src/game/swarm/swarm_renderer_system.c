#include "game/swarm/swarm_renderer_system.h"
#include "framework_ecs/ecs_core.h"
#include "raylib.h"

void SwarmRendererSystem_Draw(void) {
    // Draw Enemies
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_bIsActive[i]) {
            extern float enemy_damageFlashes[];
            Color finalColor = (enemy_damageFlashes[i] > 0.0f) ? WHITE : enemy_colors[i];
            
            DrawRectangle(
                (int)enemy_positions[i].x, 
                (int)enemy_positions[i].y, 
                (int)enemy_sizes[i], 
                (int)enemy_sizes[i], 
                finalColor
            );
        }
    }

    // Draw Projectiles
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectile_bIsActive[i]) {
            DrawRectangle(
                (int)projectile_positions[i].x, 
                (int)projectile_positions[i].y, 
                (int)projectile_sizes[i], 
                (int)projectile_sizes[i], 
                projectile_colors[i]
            );
        }
    }
}
