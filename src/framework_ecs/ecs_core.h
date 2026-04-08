#ifndef ECS_CORE_H
#define ECS_CORE_H

#include "raylib.h"
#include <stdbool.h>
#include "game/types/game_types.h"

#define MAX_COMPONENTS 100
#define MAX_ENTITIES 10000
#define MAX_ENEMIES 1000
#define MAX_PROJECTILES 1000
#define MAX_PICKUPS 1000

typedef enum {
    PICKUP_XP_GEM,
    PICKUP_NUKE,
    PICKUP_TIME_FREEZE,
    PICKUP_DOUBLE_TROUBLE,
    PICKUP_MAGNET,
    PICKUP_HEALTH
} PickupType;

typedef enum {
    PROJ_NORMAL,
    PROJ_FIREBALL,
    PROJ_BOMB,
    PROJ_SPIKE,
    PROJ_EXPLOSION // Area damage that exists for 1 frame or a short duration
} ProjectileType;

// --- ENEMY ECS ARRAYS ---
extern bool enemy_bIsActive[MAX_ENEMIES];
extern Vector2 enemy_positions[MAX_ENEMIES];
extern Vector2 enemy_velocities[MAX_ENEMIES];
extern Color enemy_colors[MAX_ENEMIES];
extern float enemy_sizes[MAX_ENEMIES];
extern int enemy_healths[MAX_ENEMIES];
extern float enemy_maxSpeeds[MAX_ENEMIES];
extern float enemy_attackTimers[MAX_ENEMIES];
extern float enemy_damageFlashes[MAX_ENEMIES];
extern int enemy_damages[MAX_ENEMIES];
extern EnemyType enemy_types[MAX_ENEMIES];

// --- PROJECTILE ECS ARRAYS ---
extern bool projectile_bIsActive[MAX_PROJECTILES];
extern Vector2 projectile_positions[MAX_PROJECTILES];
extern Vector2 projectile_velocities[MAX_PROJECTILES];
extern Color projectile_colors[MAX_PROJECTILES];
extern float projectile_sizes[MAX_PROJECTILES];
extern int projectile_damage[MAX_PROJECTILES];
extern int projectile_penetrations[MAX_PROJECTILES];
extern int projectile_damageCaps[MAX_PROJECTILES];
extern int projectile_damageDealt[MAX_PROJECTILES];
extern float projectile_timers[MAX_PROJECTILES]; // Use for life timer, detonation, etc.
extern float projectile_specialTimers[MAX_PROJECTILES]; // Use for tick rates (spikes)
extern ProjectileType projectile_types[MAX_PROJECTILES];
extern unsigned char projectile_hitMasks[MAX_PROJECTILES][125]; // (MAX_ENEMIES + 7) / 8

// --- PICKUP ECS ARRAYS ---
extern bool pickup_bIsActive[MAX_PICKUPS];
extern Vector2 pickup_positions[MAX_PICKUPS];
extern PickupType pickup_types[MAX_PICKUPS];
extern bool pickup_bIsMagnetized[MAX_PICKUPS];
extern float pickup_magnetizeSpeeds[MAX_PICKUPS];
extern int pickup_values[MAX_PICKUPS];

// --- CORE FUNCTIONS ---
void ECS_Init(void);

// Spawn functions return the index on success, -1 on array full
int ECS_SpawnEnemy(Vector2 pos, Color col, float size, int health, float maxSpeed, int damage, EnemyType type);
void ECS_DestroyEnemy(int entityId);

int ECS_SpawnProjectile(Vector2 pos, Vector2 vel, Color col, float size, int damage, int penetrations);
int ECS_SpawnProjectileEx(Vector2 pos, Vector2 vel, Color col, float size, int damage, int penetrations, ProjectileType type, float timer, int damageCap);
void ECS_DestroyProjectile(int entityId);

int ECS_SpawnPickup(Vector2 pos, PickupType type, int value);
void ECS_DestroyPickup(int entityId);

#endif // ECS_CORE_H
