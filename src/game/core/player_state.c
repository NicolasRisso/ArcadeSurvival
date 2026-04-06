#include "game/core/player_state.h"

void PlayerState_Init(PlayerState* state) {
    if (!state) return;
    HealthComponent_Init(&state->health, 100);
    state->score = 0;
    
    XPComponent_Init(&state->xp);
    
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

void PlayerState_Update(PlayerState* state, float deltaTime) {
    if (!state) return;

    if (state->damageFlashTimer > 0.0f) {
        state->damageFlashTimer -= deltaTime;
    }

    if (state->health.currentHealth <= 0) {
        state->health.bIsDead = true;
    }
}

void PlayerState_TakeDamage(PlayerState* state, int amount) {
    if (!state || state->health.bIsDead) return;
    
    // Invulnerability window: if already flashing, skip damage
    if (state->damageFlashTimer > 0.0f) return;
    
    state->health.currentHealth -= amount;
    state->damageFlashTimer = 0.5f; // 0.5s invulnerability/flash
    
    if (state->health.currentHealth < 0) {
        state->health.currentHealth = 0;
    }
}

void PlayerState_AddExperience(PlayerState* state, int amount) {
    if (!state) return;
    if (XPComponent_AddExperience(&state->xp, amount)) {
        // Level up happened, could heal or something
    }
}
