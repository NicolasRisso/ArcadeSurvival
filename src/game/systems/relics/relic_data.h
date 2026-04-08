#ifndef RELIC_DATA_H
#define RELIC_DATA_H

#include "game/systems/relics/relic_types.h"
#include <stdio.h>
#include <string.h>

static inline const char* GetRelicName(RelicType type) {
    switch (type) {
        case RELIC_HEART:     return "Health Relic";
        case RELIC_SWORD:     return "Damage Relic";
        case RELIC_HOURGLASS: return "AttackSpeed Relic";
        case RELIC_LENS:      return "Size Relic";
        case RELIC_BOOTS:     return "Speed Relic";
        case RELIC_FANG:      return "LifeSteal Relic";
        default: return "Unknown Relic";
    }
}

static inline void GetRelicDescription(RelicType type, int level, char* buffer) {
    if (level == 0) {
        switch (type) {
            case RELIC_HEART:     sprintf(buffer, "Increases Max Health by 5%%."); break;
            case RELIC_SWORD:     sprintf(buffer, "Increases All Damage by 8%%."); break;
            case RELIC_HOURGLASS: sprintf(buffer, "Increases Attack Speed by 6%%."); break;
            case RELIC_LENS:      sprintf(buffer, "Increases Area/Size by 15%%."); break;
            case RELIC_BOOTS:     sprintf(buffer, "Increases Movement Speed by 7.5%%."); break;
            case RELIC_FANG:      sprintf(buffer, "Adds 2.5%% Life Steal on damage."); break;
            default: sprintf(buffer, "A mysterious relic."); break;
        }
    } else {
        switch (type) {
            case RELIC_HEART:     sprintf(buffer, "- +5%% Max Health"); break;
            case RELIC_SWORD:     sprintf(buffer, "- +8%% Damage"); break;
            case RELIC_HOURGLASS: sprintf(buffer, "- +6%% Attack Speed"); break;
            case RELIC_LENS:      sprintf(buffer, "- +15%% Area/Size"); break;
            case RELIC_BOOTS:     sprintf(buffer, "- +7.5%% Movement Speed"); break;
            case RELIC_FANG:      sprintf(buffer, "- +2.5%% Life Steal"); break;
            default: sprintf(buffer, "Higher level."); break;
        }
    }
}

#endif // RELIC_DATA_H
