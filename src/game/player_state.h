#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include <stdbool.h>

typedef struct {
    int maxHealth;
    int currentHealth;
    int score;
    int level;
    int experience;
    bool bIsDead;
} PlayerState;

void PlayerState_Init(PlayerState* state);
void PlayerState_TakeDamage(PlayerState* state, int amount);
void PlayerState_AddExperience(PlayerState* state, int amount);

#endif // PLAYER_STATE_H
