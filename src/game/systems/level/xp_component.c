#include "game/systems/level/xp_component.h"

void XPComponent_Init(XPComponent* comp) {
    if (!comp) return;
    comp->currentLevel = 1;
    comp->currentXP = 0;
    comp->xpToNextLevel = 100; // BASE XP to reach Level 2
}

bool XPComponent_AddExperience(XPComponent* comp, int amount) {
    if (!comp) return false;
    
    comp->currentXP += amount;
    
    if (comp->currentXP >= comp->xpToNextLevel) {
        comp->currentLevel++;
        comp->currentXP -= comp->xpToNextLevel; // Carry over
        comp->xpToNextLevel = (int)(comp->xpToNextLevel * 1.5f); // Increase requirement
        return true;
    }
    
    return false;
}
