#ifndef LEVEL_UP_TYPES_H
#define LEVEL_UP_TYPES_H

#include "game/systems/combat/weapon_types.h"

typedef enum {
    OPTION_WEAPON,
    OPTION_RELIC // Placeholder for future relics
} LevelUpOptionType;

typedef struct {
    LevelUpOptionType type;
    WeaponType weaponType;
    int currentLevel;
    // Future relic fields could go here
} LevelUpOption;

#endif // LEVEL_UP_TYPES_H
