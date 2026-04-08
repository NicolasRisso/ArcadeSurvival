#include "framework_ecs/ecs_core.h"
#include <string.h>

// --- ENEMY ECS ARRAYS ---
bool enemy_bIsActive[MAX_ENEMIES];
Vector2 enemy_positions[MAX_ENEMIES];
Vector2 enemy_velocities[MAX_ENEMIES];
Color enemy_colors[MAX_ENEMIES];
float enemy_sizes[MAX_ENEMIES];
int enemy_healths[MAX_ENEMIES];
float enemy_maxSpeeds[MAX_ENEMIES];
float enemy_attackTimers[MAX_ENEMIES];
float enemy_damageFlashes[MAX_ENEMIES];
int enemy_damages[MAX_ENEMIES];
EnemyType enemy_types[MAX_ENEMIES];

// --- PROJECTILE ECS ARRAYS ---
bool projectile_bIsActive[MAX_PROJECTILES];
Vector2 projectile_positions[MAX_PROJECTILES];
Vector2 projectile_velocities[MAX_PROJECTILES];
Color projectile_colors[MAX_PROJECTILES];
float projectile_sizes[MAX_PROJECTILES];
int projectile_damage[MAX_PROJECTILES];
int projectile_penetrations[MAX_PROJECTILES];
int projectile_damageCaps[MAX_PROJECTILES];
int projectile_damageDealt[MAX_PROJECTILES];
float projectile_timers[MAX_PROJECTILES];
float projectile_specialTimers[MAX_PROJECTILES];
ProjectileType projectile_types[MAX_PROJECTILES];
unsigned char projectile_hitMasks[MAX_PROJECTILES][HIT_MASK_SIZE];

// --- PICKUP ECS ARRAYS ---
bool pickup_bIsActive[MAX_PICKUPS];
Vector2 pickup_positions[MAX_PICKUPS];
PickupType pickup_types[MAX_PICKUPS];
bool pickup_bIsMagnetized[MAX_PICKUPS];
float pickup_magnetizeSpeeds[MAX_PICKUPS];
int pickup_values[MAX_PICKUPS];

void ECS_Init(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemy_bIsActive[i] = false;
        enemy_sizes[i] = 10.0f;
        enemy_attackTimers[i] = 0.0f;
        enemy_damageFlashes[i] = 0.0f;
    }
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectile_bIsActive[i] = false;
        projectile_sizes[i] = 5.0f;
        projectile_timers[i] = 0.0f;
        projectile_specialTimers[i] = 0.0f;
        projectile_damageCaps[i] = 0;
        projectile_damageDealt[i] = 0;
        projectile_types[i] = PROJ_NORMAL;
        memset(projectile_hitMasks[i], 0, sizeof(projectile_hitMasks[i]));
    }
    for (int i = 0; i < MAX_PICKUPS; i++) {
        pickup_bIsActive[i] = false;
        pickup_bIsMagnetized[i] = false;
    }
}

int ECS_SpawnEnemy(Vector2 pos, Color col, float size, int health, float maxSpeed, int damage, EnemyType type) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemy_bIsActive[i]) {
            enemy_positions[i] = pos;
            enemy_velocities[i] = (Vector2){0, 0};
            enemy_colors[i] = col;
            enemy_sizes[i] = size;
            enemy_healths[i] = health;
            enemy_maxSpeeds[i] = maxSpeed;
            enemy_damages[i] = damage;
            enemy_types[i] = type;
            enemy_attackTimers[i] = 0.0f;
            enemy_damageFlashes[i] = 0.0f;
            enemy_bIsActive[i] = true;
            return i;
        }
    }
    return -1;
}

void ECS_DestroyEnemy(int entityId) {
    if (entityId >= 0 && entityId < MAX_ENEMIES) {
        enemy_bIsActive[entityId] = false;
    }
}

int ECS_SpawnProjectile(Vector2 pos, Vector2 vel, Color col, float size, int damage, int penetrations) {
    return ECS_SpawnProjectileEx(pos, vel, col, size, damage, penetrations, PROJ_NORMAL, 0.0f, 2147483647); // INT_MAX
}

int ECS_SpawnProjectileEx(Vector2 pos, Vector2 vel, Color col, float size, int damage, int penetrations, ProjectileType type, float timer, int damageCap) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectile_bIsActive[i]) {
            projectile_positions[i] = pos;
            projectile_velocities[i] = vel;
            projectile_colors[i] = col;
            projectile_sizes[i] = size;
            projectile_damage[i] = damage;
            projectile_penetrations[i] = penetrations;
            projectile_damageCaps[i] = damageCap;
            projectile_damageDealt[i] = 0;
            projectile_types[i] = type;
            projectile_timers[i] = timer;
            projectile_specialTimers[i] = 0.0f;
            memset(projectile_hitMasks[i], 0, sizeof(projectile_hitMasks[i]));
            projectile_bIsActive[i] = true;
            return i;
        }
    }
    return -1;
}

void ECS_DestroyProjectile(int entityId) {
    if (entityId >= 0 && entityId < MAX_PROJECTILES) {
        projectile_bIsActive[entityId] = false;
    }
}

int ECS_SpawnPickup(Vector2 pos, PickupType type, int value) {
    for (int i = 0; i < MAX_PICKUPS; i++) {
        if (!pickup_bIsActive[i]) {
            pickup_positions[i] = pos;
            pickup_types[i] = type;
            pickup_values[i] = value;
            pickup_bIsMagnetized[i] = false;
            pickup_magnetizeSpeeds[i] = 0.0f;
            pickup_bIsActive[i] = true;
            return i;
        }
    }
    return -1;
}

void ECS_DestroyPickup(int entityId) {
    if (entityId >= 0 && entityId < MAX_PICKUPS) {
        pickup_bIsActive[entityId] = false;
    }
}
