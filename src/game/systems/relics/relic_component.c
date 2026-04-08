#include "game/systems/relics/relic_component.h"

void RelicComponent_Init(RelicComponent* comp) {
    if (!comp) return;
    comp->activeRelicCount = 0;
    for (int i = 0; i < MAX_RELICS; i++) {
        comp->relics[i].type = RELIC_NONE;
        comp->relics[i].level = 0;
    }
}

bool RelicComponent_HasRelic(RelicComponent* comp, RelicType type) {
    if (!comp) return false;
    for (int i = 0; i < comp->activeRelicCount; i++) {
        if (comp->relics[i].type == type) return true;
    }
    return false;
}

void RelicComponent_UpgradeOrAdd(RelicComponent* comp, RelicType type) {
    if (!comp || type == RELIC_NONE) return;

    // Check if we already have it
    for (int i = 0; i < comp->activeRelicCount; i++) {
        if (comp->relics[i].type == type) {
            if (comp->relics[i].level < MAX_RELIC_LEVEL) {
                comp->relics[i].level++;
            }
            return;
        }
    }

    // New relic
    if (comp->activeRelicCount < MAX_RELICS) {
        comp->relics[comp->activeRelicCount].type = type;
        comp->relics[comp->activeRelicCount].level = 1;
        comp->activeRelicCount++;
    }
}
