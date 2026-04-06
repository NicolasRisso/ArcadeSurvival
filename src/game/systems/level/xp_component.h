#ifndef XP_COMPONENT_H
#define XP_COMPONENT_H

typedef struct {
    int currentLevel;
    int currentXP;
    int xpToNextLevel;
} XPComponent;

void XPComponent_Init(XPComponent* comp);
// Returns true if the player leveled up
#include <stdbool.h>
bool XPComponent_AddExperience(XPComponent* comp, int amount);

#endif // XP_COMPONENT_H
