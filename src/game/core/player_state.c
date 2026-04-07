#include "game/core/player_state.h"
#include "game/systems/combat/weapon_selector.h"
#include "raylib.h"

void PlayerState_Init(PlayerState* state) {
    if (!state) return;
    HealthComponent_Init(&state->health, 100);
    state->score = 0;
    
    XPComponent_Init(&state->xp);
    
    WeaponComponent_Init(&state->weapons);
    state->gameTime = 0.0f;
    
    // Start with Crystal Shard
    WeaponComponent_UpgradeOrAdd(&state->weapons, WEAPON_CRYSTAL_SHARD);
}

void PlayerState_Update(PlayerState* state, float deltaTime) {
    if (!state) return;

    if (state->damageFlashTimer > 0.0f) {
        state->damageFlashTimer -= deltaTime;
    }

    if (state->health.currentHealth <= 0) {
        state->health.bIsDead = true;
    }

    state->gameTime += deltaTime;
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
        // Level up happened!
        state->levelUpOptionCount = WeaponSelector_GetRandomOptions(state, state->levelUpOptions);
        if (state->levelUpOptionCount > 0) {
            state->bIsLevelingUp = true;
            EnableCursor();
            ShowCursor();
        }
    }
}
