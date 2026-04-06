#include "game/core/player_state.h"

void PlayerState_Init(PlayerState* state) {
    if (!state) return;
    HealthComponent_Init(&state->health, 100);
    state->score = 0;
    state->level = 1;
    state->experience = 0;
    
    WeaponComponent_Init(&state->weapons);
    
    Weapon basicArrow = {0};
    basicArrow.type = WEAPON_ARROW;
    basicArrow.level = 1;
    basicArrow.damage = 15;
    basicArrow.penetration = 2;
    basicArrow.fireRate = 1.0f; // 1 shot per second
    basicArrow.cooldownTimer = 0.0f; // ready to fire immediately
    
    WeaponComponent_AddWeapon(&state->weapons, basicArrow);
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
