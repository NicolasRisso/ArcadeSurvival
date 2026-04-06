#ifndef WEAPON_TYPES_H
#define WEAPON_TYPES_H

typedef enum {
    WEAPON_NONE = 0,
    WEAPON_ARROW
} WeaponType;

typedef struct {
    WeaponType type;
    int level;
    int damage;
    int penetration;
    float fireRate;      // Time between shots in seconds
    float cooldownTimer; // Current timer tracking when we can shoot again
} Weapon;

#endif // WEAPON_TYPES_H
