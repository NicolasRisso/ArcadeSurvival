#ifndef WEAPON_DATA_H
#define WEAPON_DATA_H

#include "game/systems/combat/weapon_types.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int damage;
    float fireRate;
    int penetration;
    int projectileCount;
    float range;
    float specialValue; // Explosion radius, tick rate, etc.
    int damageCap;      // Total damage allowed before projectile vanishes (for spikes)
} WeaponLevelStats;

// FIREBALL WAND STATS (Every 8s -> 3s, Damage up, Explosion Range up)
static const WeaponLevelStats FIREBALL_WAND_LEVELS[MAX_WEAPON_LEVEL + 1] = {
    {0}, // Level 0 (None)
    {15, 8.0f, 1, 4, 100.0f, 40.0f, 2147483647},  // Level 1
    {18, 7.5f, 1, 4, 100.0f, 42.0f, 2147483647},  // Level 2
    {21, 7.0f, 1, 4, 100.0f, 44.0f, 2147483647},  // Level 3
    {24, 6.5f, 1, 4, 100.0f, 46.0f, 2147483647},  // Level 4
    {27, 6.0f, 2, 8, 110.0f, 50.0f, 2147483647},  // Level 5 (8 fireballs, 2 each axis)
    {30, 5.5f, 2, 8, 110.0f, 52.0f, 2147483647},  // Level 6
    {33, 5.2f, 2, 8, 110.0f, 54.0f, 2147483647},  // Level 7
    {36, 4.9f, 2, 8, 110.0f, 56.0f, 2147483647},  // Level 8
    {40, 4.6f, 2, 8, 110.0f, 58.0f, 2147483647},  // Level 9
    {45, 4.3f, 3, 12, 120.0f, 65.0f, 2147483647}, // Level 10 (12 fireballs, 3 each axis)
    {50, 4.0f, 3, 12, 120.0f, 67.0f, 2147483647}, // Level 11
    {55, 3.8f, 3, 12, 120.0f, 69.0f, 2147483647}, // Level 12
    {60, 3.6f, 3, 12, 120.0f, 71.0f, 2147483647}, // Level 13
    {65, 3.3f, 3, 12, 120.0f, 73.0f, 2147483647}, // Level 14
    {75, 3.0f, 4, 16, 130.0f, 85.0f, 2147483647}  // Level 15 (16 fireballs, 4 each axis)
};

// CRYSTAL SHARD STATS (Closest enemy, penetration, multiple projectiles)
static const WeaponLevelStats CRYSTAL_SHARD_LEVELS[MAX_WEAPON_LEVEL + 1] = {
    {0},
    {20, 3.0f, 2, 1, 0.0f, 0.0f, 2147483647},     // Buffed Level 1 (Damage 10->20, Rate 4->3, Pen 1->2)
    {24, 2.8f, 2, 1, 0.0f, 0.0f, 2147483647},     // Level 2
    {28, 2.6f, 3, 1, 0.0f, 0.0f, 2147483647},     // Level 3
    {35, 2.4f, 4, 2, 0.0f, 0.0f, 2147483647},     // Level 4 (2 projectiles)
    {42, 2.2f, 5, 2, 0.0f, 0.0f, 2147483647},     // Level 5
    {49, 2.0f, 6, 2, 0.0f, 0.0f, 2147483647},     // Level 6
    {56, 1.8f, 7, 2, 0.0f, 0.0f, 2147483647},     // Level 7
    {65, 1.6f, 8, 3, 0.0f, 0.0f, 2147483647},     // Level 8 (3 projectiles)
    {74, 1.4f, 9, 3, 0.0f, 0.0f, 2147483647},     // Level 9
    {83, 1.2f, 10, 3, 0.0f, 0.0f, 2147483647},    // Level 10
    {92, 1.1f, 11, 3, 0.0f, 0.0f, 2147483647},    // Level 11
    {110, 1.0f, 12, 4, 0.0f, 0.0f, 2147483647},   // Level 12 (4 projectiles)
    {130, 0.9f, 13, 4, 0.0f, 0.0f, 2147483647},   // Level 13
    {155, 0.8f, 14, 4, 0.0f, 0.0f, 2147483647},   // Level 14
    {200, 0.6f, 15, 6, 0.0f, 0.0f, 2147483647}    // Level 15 (Damage up to 200, Rate 0.6f)
};

// DEATH AURA STATS (Damage every 0.2s -> 0.1s, range up)
static const WeaponLevelStats DEATH_AURA_LEVELS[MAX_WEAPON_LEVEL + 1] = {
    {0},
    {5, 0.2f, 0, 0, 80.0f, 0.0f, 2147483647},     // Level 1
    {6, 0.2f, 0, 0, 85.0f, 0.0f, 2147483647},     // Level 2
    {7, 0.2f, 0, 0, 90.0f, 0.0f, 2147483647},     // Level 3
    {8, 0.18f, 0, 0, 100.0f, 0.0f, 2147483647},   // Level 4
    {10, 0.18f, 0, 0, 105.0f, 0.0f, 2147483647},  // Level 5
    {12, 0.18f, 0, 0, 110.0f, 0.0f, 2147483647},  // Level 6
    {14, 0.16f, 0, 0, 120.0f, 0.0f, 2147483647},  // Level 7
    {16, 0.16f, 0, 0, 125.0f, 0.0f, 2147483647},  // Level 8
    {18, 0.16f, 0, 0, 130.0f, 0.0f, 2147483647},  // Level 9
    {22, 0.14f, 0, 0, 145.0f, 0.0f, 2147483647},  // Level 10
    {26, 0.14f, 0, 0, 150.0f, 0.0f, 2147483647},  // Level 11
    {30, 0.14f, 0, 0, 155.0f, 0.0f, 2147483647},  // Level 12
    {35, 0.12f, 0, 0, 170.0f, 0.0f, 2147483647},  // Level 13
    {40, 0.12f, 0, 0, 175.0f, 0.0f, 2147483647},  // Level 14
    {55, 0.1f, 0, 0, 200.0f, 0.0f, 2147483647}    // Level 15
};

// BOMB SHOES STATS (Drops every 5s -> 1s, Bigger radius, High damage)
static const WeaponLevelStats BOMB_SHOES_LEVELS[MAX_WEAPON_LEVEL + 1] = {
    {0},
    {100, 5.0f, 1, 1, 80.0f, 3.0f, 2147483647},    // Level 1 (3s explode delay)
    {110, 4.8f, 1, 1, 85.0f, 3.0f, 2147483647},    // Level 2
    {120, 4.6f, 1, 1, 90.0f, 3.0f, 2147483647},    // Level 3
    {140, 4.3f, 1, 1, 100.0f, 3.0f, 2147483647},   // Level 4
    {160, 4.0f, 1, 1, 105.0f, 2.8f, 2147483647},   // Level 5
    {180, 3.8f, 1, 1, 110.0f, 2.8f, 2147483647},   // Level 6
    {210, 3.5f, 1, 1, 120.0f, 2.6f, 2147483647},   // Level 7
    {240, 3.2f, 1, 1, 125.0f, 2.6f, 2147483647},   // Level 8
    {270, 2.9f, 1, 1, 130.0f, 2.4f, 2147483647},   // Level 9
    {320, 2.5f, 1, 1, 145.0f, 2.2f, 2147483647},   // Level 10
    {370, 2.2f, 1, 1, 150.0f, 2.2f, 2147483647},   // Level 11
    {420, 1.9f, 1, 1, 155.0f, 2.0f, 2147483647},   // Level 12
    {480, 1.6f, 1, 1, 170.0f, 1.8f, 2147483647},   // Level 13
    {550, 1.3f, 1, 1, 175.0f, 1.8f, 2147483647},   // Level 14
    {750, 1.0f, 1, 1, 200.0f, 1.5f, 2147483647}    // Level 15
};

// NATURE SPIKES STATS (Summon every 2s -> 0.5s, lasts 2s -> 3s, tick 0.2s -> 0.15s)
static const WeaponLevelStats NATURE_SPIKES_LEVELS[MAX_WEAPON_LEVEL + 1] = {
    {0},
    {20, 2.0f, 0, 1, 20.0f, 0.2f, 100},    // Level 1 (100 total damage cap)
    {22, 1.9f, 0, 1, 21.0f, 0.2f, 150},    // Level 2
    {24, 1.8f, 0, 1, 22.0f, 0.2f, 200},    // Level 3
    {28, 1.6f, 0, 1, 23.0f, 0.2f, 300},    // Level 4
    {32, 1.4f, 0, 1, 25.0f, 0.18f, 400},   // Level 5
    {36, 1.3f, 0, 1, 26.0f, 0.18f, 500},   // Level 6
    {42, 1.2f, 0, 1, 27.0f, 0.18f, 650},   // Level 7
    {48, 1.1f, 0, 1, 28.0f, 0.18f, 800},   // Level 8
    {55, 1.0f, 0, 1, 30.0f, 0.18f, 1000},  // Level 9
    {65, 0.9f, 0, 1, 32.0f, 0.15f, 1250},  // Level 10
    {75, 0.85f, 0, 1, 33.0f, 0.15f, 1500}, // Level 11
    {85, 0.8f, 0, 1, 34.0f, 0.15f, 1800},  // Level 12
    {100, 0.7f, 0, 1, 35.0f, 0.15f, 2200}, // Level 13
    {120, 0.6f, 0, 1, 37.0f, 0.15f, 2600}, // Level 14
    {150, 0.5f, 0, 1, 45.0f, 0.15f, 3500}  // Level 15
};

static inline const WeaponLevelStats* GetWeaponStats(WeaponType type, int level) {
    if (level < 1) return NULL;
    if (level > MAX_WEAPON_LEVEL) level = MAX_WEAPON_LEVEL;
    
    switch (type) {
        case WEAPON_FIREBALL_WAND: return &FIREBALL_WAND_LEVELS[level];
        case WEAPON_CRYSTAL_SHARD: return &CRYSTAL_SHARD_LEVELS[level];
        case WEAPON_DEATH_AURA:    return &DEATH_AURA_LEVELS[level];
        case WEAPON_BOMB_SHOES:    return &BOMB_SHOES_LEVELS[level];
        case WEAPON_NATURE_SPIKES: return &NATURE_SPIKES_LEVELS[level];
        default: return NULL;
    }
}

static inline const char* GetWeaponName(WeaponType type) {
    switch (type) {
        case WEAPON_FIREBALL_WAND: return "Fireball Wand";
        case WEAPON_CRYSTAL_SHARD: return "Crystal Shard";
        case WEAPON_DEATH_AURA:    return "Death Aura";
        case WEAPON_BOMB_SHOES:    return "Bomb Shoes";
        case WEAPON_NATURE_SPIKES: return "Nature Spikes";
        default: return "Unknown";
    }
}

static inline void GetWeaponDescription(WeaponType type, int level, char* buffer) {
    if (level == 0) {
        // Level 1 description
        switch (type) {
            case WEAPON_FIREBALL_WAND: sprintf(buffer, "Fires fireballs in 4 axes\nthat explode."); break;
            case WEAPON_CRYSTAL_SHARD: sprintf(buffer, "Fires at the closest enemy\nwith penetration."); break;
            case WEAPON_DEATH_AURA:    sprintf(buffer, "Deals damage in an aura\naround you."); break;
            case WEAPON_BOMB_SHOES:    sprintf(buffer, "Drops bombs at your feet\nperiodically."); break;
            case WEAPON_NATURE_SPIKES: sprintf(buffer, "Summons spikes at random\nenemy feet."); break;
            default: sprintf(buffer, "A mysterious weapon."); break;
        }
    } else {
        const WeaponLevelStats* current = GetWeaponStats(type, level);
        const WeaponLevelStats* next = GetWeaponStats(type, level + 1);
        if (!next) {
            sprintf(buffer, "Max level reached.");
            return;
        }

        // Generate dynamic description of what improves
        char changes[256] = "";
        if (next->damage > current->damage) sprintf(changes + strlen(changes), "- +%d Damage\n", next->damage - current->damage);
        if (next->fireRate < current->fireRate) sprintf(changes + strlen(changes), "- -%.1fs Fire Rate\n", current->fireRate - next->fireRate);
        if (next->penetration > current->penetration) sprintf(changes + strlen(changes), "- +%d Pierce\n", next->penetration - current->penetration);
        if (next->projectileCount > current->projectileCount) sprintf(changes + strlen(changes), "- +%d Projectiles\n", next->projectileCount - current->projectileCount);
        if (next->range > current->range) sprintf(changes + strlen(changes), "- +%.0f Range\n", next->range - current->range);
        if (next->specialValue != current->specialValue) {
            if (type == WEAPON_FIREBALL_WAND || type == WEAPON_BOMB_SHOES) sprintf(changes + strlen(changes), "- +%.0f AOE Radius\n", next->specialValue - current->specialValue);
            else if (type == WEAPON_NATURE_SPIKES) sprintf(changes + strlen(changes), "- -%.2fs Tick Rate\n", current->specialValue - next->specialValue);
        }
        
        sprintf(buffer, "%s", changes);
    }
}

#endif // WEAPON_DATA_H
