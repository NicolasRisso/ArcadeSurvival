#ifndef WEAPON_SELECTOR_H
#define WEAPON_SELECTOR_H

#include "game/systems/level/level_up_types.h"

// Forward declaration to break circular dependency
struct PlayerState;
typedef struct PlayerState PlayerState;

// Fills outOptions with up to 3 random selection options. 
// Returns the number of options found.
int WeaponSelector_GetRandomOptions(PlayerState* state, LevelUpOption outOptions[3]);

#endif // WEAPON_SELECTOR_H
