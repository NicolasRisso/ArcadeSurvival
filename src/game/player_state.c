#include "game/player_state.h"

void PlayerState_Init(PlayerState* state) {
    if (!state) return;
    state->maxHealth = 100;
    state->currentHealth = 100;
    state->score = 0;
    state->level = 1;
    state->experience = 0;
    state->bIsDead = false;
}

void PlayerState_TakeDamage(PlayerState* state, int amount) {
    if (!state || state->bIsDead) return;
    state->currentHealth -= amount;
    if (state->currentHealth <= 0) {
        state->currentHealth = 0;
        state->bIsDead = true;
    }
}

void PlayerState_AddExperience(PlayerState* state, int amount) {
    if (!state || state->bIsDead) return;
    state->experience += amount;
    // VERY simple level up logic for now
    if (state->experience >= state->level * 100) {
        state->experience = 0;
        state->level++;
        state->maxHealth += 10;
        state->currentHealth = state->maxHealth; // Full heal on level up
    }
}
