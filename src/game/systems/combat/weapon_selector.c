#include "game/systems/combat/weapon_selector.h"
#include "game/core/player_state.h"
#include "game/systems/combat/weapon_component.h"
#include "game/systems/combat/weapon_types.h"
#include "game/systems/relics/relic_component.h"
#include "game/systems/relics/relic_types.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>

int WeaponSelector_GetRandomOptions(PlayerState* state, LevelUpOption outOptions[3]) {
    if (!state) return 0;

    LevelUpOption candidates[20]; // More candidates now
    int candidateCount = 0;

    WeaponType allTypes[] = {
        WEAPON_FIREBALL_WAND,
        WEAPON_CRYSTAL_SHARD,
        WEAPON_DEATH_AURA,
        WEAPON_BOMB_SHOES,
        WEAPON_NATURE_SPIKES
    };

    bool hasSpace = (state->weapons.activeWeaponCount < MAX_WEAPONS);

    for (int i = 0; i < 5; i++) {
        WeaponType type = allTypes[i];
        
        // Find if player has it
        int existingIdx = -1;
        for (int w = 0; w < state->weapons.activeWeaponCount; w++) {
            if (state->weapons.weapons[w].type == type) {
                existingIdx = w;
                break;
            }
        }

        if (existingIdx != -1) {
            // Already has it, upgrade if not max level
            if (state->weapons.weapons[existingIdx].level < MAX_WEAPON_LEVEL) {
                candidates[candidateCount++] = (LevelUpOption){
                    .type = OPTION_WEAPON,
                    .weaponType = type,
                    .currentLevel = state->weapons.weapons[existingIdx].level
                };
            }
        } else if (hasSpace) {
            // New weapon
            candidates[candidateCount++] = (LevelUpOption){
                .type = OPTION_WEAPON,
                .weaponType = type,
                .currentLevel = 0
            };
        }
    }

    // --- Relic Candidates ---
    RelicType allRelics[] = {
        RELIC_HEART,
        RELIC_SWORD,
        RELIC_HOURGLASS,
        RELIC_LENS,
        RELIC_BOOTS,
        RELIC_FANG
    };

    bool hasRelicSpace = (state->relics.activeRelicCount < MAX_RELICS);

    for (int i = 0; i < 6; i++) {
        RelicType type = allRelics[i];
        
        // Find if player has it
        int existingIdx = -1;
        for (int r = 0; r < state->relics.activeRelicCount; r++) {
            if (state->relics.relics[r].type == type) {
                existingIdx = r;
                break;
            }
        }

        if (existingIdx != -1) {
            // Already has it, upgrade if not max level
            if (state->relics.relics[existingIdx].level < MAX_RELIC_LEVEL) {
                candidates[candidateCount++] = (LevelUpOption){
                    .type = OPTION_RELIC,
                    .relicType = type,
                    .currentLevel = state->relics.relics[existingIdx].level
                };
            }
        } else if (hasRelicSpace) {
            // New relic
            candidates[candidateCount++] = (LevelUpOption){
                .type = OPTION_RELIC,
                .relicType = type,
                .currentLevel = 0
            };
        }
    }

    if (candidateCount == 0) return 0;

    // Pick unique random options
    int countToPick = (candidateCount < 3) ? candidateCount : 3;
    
    // Shuffle candidates briefly (Fisher-Yates)
    for (int i = candidateCount - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);
        LevelUpOption temp = candidates[i];
        candidates[i] = candidates[j];
        candidates[j] = temp;
    }

    for (int i = 0; i < countToPick; i++) {
        outOptions[i] = candidates[i];
    }

    return countToPick;
}
