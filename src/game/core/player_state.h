#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "game/systems/health/health_component.h"
#include "game/systems/combat/weapon_component.h"
#include "game/systems/level/xp_component.h"

#include "game/systems/level/level_up_types.h"
#include <stdbool.h>

typedef struct PlayerState {
    HealthComponent health;
    int score;
    XPComponent xp;
    
    float damageFlashTimer;
    
    WeaponComponent weapons;
    
    // Level Selection State
    bool bIsLevelingUp;
    LevelUpOption levelUpOptions[3];
    int levelUpOptionCount;
    
    float gameTime;
} PlayerState;

void PlayerState_Init(PlayerState* state);
void PlayerState_Update(PlayerState* state, float deltaTime);
void PlayerState_TakeDamage(PlayerState* state, int amount);
void PlayerState_AddExperience(PlayerState* state, int amount);

#endif // PLAYER_STATE_H
