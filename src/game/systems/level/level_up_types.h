#ifndef LEVEL_UP_TYPES_H
#define LEVEL_UP_TYPES_H

#include "game/systems/combat/weapon_types.h"
#include "game/systems/relics/relic_types.h"

typedef enum {
    OPTION_WEAPON,
    OPTION_RELIC
} LevelUpOptionType;

typedef struct {
    LevelUpOptionType type;
    union {
        WeaponType weaponType;
        RelicType relicType;
    };
    int currentLevel;
} LevelUpOption;

#endif // LEVEL_UP_TYPES_H
