#include "game/core/player_state.h"

void PlayerState_Init(PlayerState* state) {
    if (!state) return;
    HealthComponent_Init(&state->health, 100);
    state->score = 0;
    state->level = 1;
    state->experience = 0;
}

void PlayerState_TakeDamage(PlayerState* state, int amount) {
    if (!state) return;
    HealthComponent_TakeDamage(&state->health, amount);
}

void PlayerState_AddExperience(PlayerState* state, int amount) {
    if (!state || state->health.bIsDead) return;
    state->experience += amount;
    // VERY simple level up logic for now
    if (state->experience >= state->level * 100) {
        state->experience = 0;
        state->level++;
        state->health.maxHealth += 10;
        HealthComponent_Heal(&state->health, state->health.maxHealth); // Full heal on level up
    }
}
