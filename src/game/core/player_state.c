#include "game/core/player_state.h"
#include "game/systems/combat/weapon_selector.h"
#include "game/audio/audio_manager.h"
#include "graphics/resource_manager.h"
#include "raylib.h"
#include <stdio.h>

void PlayerState_Init(PlayerState* state) {
    if (!state) return;
    HealthComponent_Init(&state->health, 100);
    state->score = 0;
    
    XPComponent_Init(&state->xp);
    
    WeaponComponent_Init(&state->weapons);
    RelicComponent_Init(&state->relics);
    
    // Initialize stats
    state->stats.extraHealth = 0;
    state->stats.extraDamage = 0;
    state->stats.extraAttackSpeed = 0;
    state->stats.extraSize = 0;
    state->stats.extraMovement = 0;
    state->stats.extraLifeSteal = 0;
    
    state->statNotify.timer = 0;
    state->statNotify.message[0] = '\0';

    PlayerState_RecalculateStats(state);
    
    state->gameTime = 0.0f;
    state->uiSelectedIndex = 0;
    state->bossWarningTimer = 0.0f;
    
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

    if (state->statNotify.timer > 0) {
        state->statNotify.timer -= deltaTime;
    }

    state->gameTime += deltaTime;
}

void PlayerState_TakeDamage(PlayerState* state, int amount) {
    if (!state || state->health.bIsDead) return;
    
    // Invulnerability window: if already flashing, skip damage
    if (state->damageFlashTimer > 0.0f) return;
    
    state->health.currentHealth -= amount;
    state->damageFlashTimer = 0.5f; // 0.5s invulnerability/flash
    AudioManager_PlaySfx(SND_PLAYER_HIT);
    
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
            state->uiSelectedIndex = 0; // Reset for Nav
            AudioManager_PlaySfx(SND_LEVEL_UP);
        } else {
            // Nothing to upgrade! Auto-increase a random stat
            int r = GetRandomValue(0, 5);
            float bonus = GetRandomValue(5, 10) / 1000.0f; // 0.5% to 1.0%
            const char* statName = "Unknown";

            switch (r) {
                case 0: state->stats.extraHealth += bonus; statName = "Health"; break;
                case 1: state->stats.extraDamage += bonus; statName = "Damage"; break;
                case 2: state->stats.extraAttackSpeed += bonus; statName = "Attack Speed"; break;
                case 3: state->stats.extraSize += bonus; statName = "Size"; break;
                case 4: state->stats.extraMovement += bonus; statName = "Movement"; break;
                case 5: state->stats.extraLifeSteal += bonus; statName = "Life Steal"; break;
            }

            PlayerState_RecalculateStats(state);
            sprintf(state->statNotify.message, "%s increased by %.1f%%!", statName, bonus * 100.0f);
            state->statNotify.timer = 5.0f;
        }
    }
}

void PlayerState_RecalculateStats(PlayerState* state) {
    if (!state) return;

    // Base multipliers
    state->stats.healthMultiplier = 1.0f + state->stats.extraHealth;
    state->stats.damageMultiplier = 1.0f + state->stats.extraDamage;
    state->stats.attackSpeedMultiplier = 1.0f + state->stats.extraAttackSpeed;
    state->stats.sizeMultiplier = 1.0f + state->stats.extraSize;
    state->stats.movementMultiplier = 1.0f + state->stats.extraMovement;
    state->stats.lifeSteal = 0.0f + state->stats.extraLifeSteal;

    // Relic bonuses
    for (int i = 0; i < state->relics.activeRelicCount; i++) {
        Relic r = state->relics.relics[i];
        switch (r.type) {
            case RELIC_HEART:     state->stats.healthMultiplier += r.level * 0.05f; break;
            case RELIC_SWORD:     state->stats.damageMultiplier += r.level * 0.08f; break;
            case RELIC_HOURGLASS: state->stats.attackSpeedMultiplier += r.level * 0.06f; break;
            case RELIC_BOOTS:     state->stats.movementMultiplier += r.level * 0.075f; break;
            case RELIC_LENS:      state->stats.sizeMultiplier += r.level * 0.15f; break;
            case RELIC_FANG:      state->stats.lifeSteal += r.level * 0.025f; break;
            default: break;
        }
    }

    // Dynamic HP adjustments
    int oldMax = state->health.maxHealth;
    state->health.maxHealth = (int)(100 * state->stats.healthMultiplier);
    if (state->health.maxHealth > oldMax && oldMax > 0) {
        state->health.currentHealth += (state->health.maxHealth - oldMax);
    }
}
