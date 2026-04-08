#ifndef RELIC_COMPONENT_H
#define RELIC_COMPONENT_H

#include "game/systems/relics/relic_types.h"
#include <stdbool.h>

typedef struct {
    Relic relics[MAX_RELICS];
    int activeRelicCount;
} RelicComponent;

void RelicComponent_Init(RelicComponent* comp);
void RelicComponent_UpgradeOrAdd(RelicComponent* comp, RelicType type);
bool RelicComponent_HasRelic(RelicComponent* comp, RelicType type);

#endif // RELIC_COMPONENT_H
