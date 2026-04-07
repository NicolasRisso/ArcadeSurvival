#ifndef WEAPON_TYPES_H
#define WEAPON_TYPES_H

#define MAX_WEAPON_LEVEL 15

typedef enum {
    WEAPON_NONE = 0,
    WEAPON_FIREBALL_WAND,
    WEAPON_CRYSTAL_SHARD,
    WEAPON_DEATH_AURA,
    WEAPON_BOMB_SHOES,
    WEAPON_NATURE_SPIKES
} WeaponType;

typedef struct {
    WeaponType type;
    int level;
    float cooldownTimer; // Current timer tracking when we can shoot again
} Weapon;

#endif // WEAPON_TYPES_H
