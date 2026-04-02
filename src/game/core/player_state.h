#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "game/systems/health/health_component.h"

typedef struct {
    HealthComponent health;
    int score;
    int level;
    int experience;
} PlayerState;

void PlayerState_Init(PlayerState* state);
void PlayerState_TakeDamage(PlayerState* state, int amount);
void PlayerState_AddExperience(PlayerState* state, int amount);

#endif // PLAYER_STATE_H
