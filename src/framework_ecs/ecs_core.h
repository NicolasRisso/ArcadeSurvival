#ifndef ECS_CORE_H
#define ECS_CORE_H

#include "raylib.h"
#include <stdbool.h>

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
    PICKUP_MAGNET
} PickupType;

// --- ENEMY ECS ARRAYS ---
extern bool enemy_bIsActive[MAX_ENEMIES];
extern Vector2 enemy_positions[MAX_ENEMIES];
extern Vector2 enemy_velocities[MAX_ENEMIES];
extern Color enemy_colors[MAX_ENEMIES];
extern float enemy_sizes[MAX_ENEMIES];
extern int enemy_healths[MAX_ENEMIES];
extern float enemy_maxSpeeds[MAX_ENEMIES];

// --- PROJECTILE ECS ARRAYS ---
extern bool projectile_bIsActive[MAX_PROJECTILES];
extern Vector2 projectile_positions[MAX_PROJECTILES];
extern Vector2 projectile_velocities[MAX_PROJECTILES];
extern Color projectile_colors[MAX_PROJECTILES];
extern float projectile_sizes[MAX_PROJECTILES];
extern int projectile_damage[MAX_PROJECTILES];
extern int projectile_penetrations[MAX_PROJECTILES];

// --- PICKUP ECS ARRAYS ---
extern bool pickup_bIsActive[MAX_PICKUPS];
extern Vector2 pickup_positions[MAX_PICKUPS];
extern PickupType pickup_types[MAX_PICKUPS];
extern bool pickup_bIsMagnetized[MAX_PICKUPS];
extern float pickup_magnetizeSpeeds[MAX_PICKUPS];

// --- CORE FUNCTIONS ---
void ECS_Init(void);

// Spawn functions return the index on success, -1 on array full
int ECS_SpawnEnemy(Vector2 pos, Color col, float size, int health, float maxSpeed);
void ECS_DestroyEnemy(int entityId);

int ECS_SpawnProjectile(Vector2 pos, Vector2 vel, Color col, float size, int damage, int penetrations);
void ECS_DestroyProjectile(int entityId);

int ECS_SpawnPickup(Vector2 pos, PickupType type);
void ECS_DestroyPickup(int entityId);

#endif // ECS_CORE_H
