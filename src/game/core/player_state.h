#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "game/systems/health/health_component.h"
#include "game/systems/combat/weapon_component.h"
#include "game/systems/level/xp_component.h"

#include "game/systems/level/level_up_types.h"
#include "game/systems/relics/relic_component.h"
#include <stdbool.h>

typedef struct PlayerStats {
    float healthMultiplier;
    float damageMultiplier;
    float attackSpeedMultiplier;
    float sizeMultiplier;
    float movementMultiplier;
    float lifeSteal;
    
    // Extra bonuses from auto-leveling (additive to multipliers)
    float extraHealth;
    float extraDamage;
    float extraAttackSpeed;
    float extraSize;
    float extraMovement;
    float extraLifeSteal;
} PlayerStats;

typedef struct {
    char message[64];
    float timer;
} StatNotification;

typedef struct PlayerState {
    HealthComponent health;
    int score;
    XPComponent xp;
    
    float damageFlashTimer;
    
    WeaponComponent weapons;
    RelicComponent relics;
    PlayerStats stats;
    
    // Level Selection State
    bool bIsLevelingUp;
    LevelUpOption levelUpOptions[3];
    int levelUpOptionCount;
    
    float gameTime;
    StatNotification statNotify;
    bool bShowInventoryOverlay;
} PlayerState;

void PlayerState_Init(PlayerState* state);
void PlayerState_Update(PlayerState* state, float deltaTime);
void PlayerState_TakeDamage(PlayerState* state, int amount);
void PlayerState_AddExperience(PlayerState* state, int amount);
void PlayerState_RecalculateStats(PlayerState* state);

#endif // PLAYER_STATE_H
