#ifndef WEAPON_COMPONENT_H
#define WEAPON_COMPONENT_H

#include "game/systems/combat/weapon_types.h"

#define MAX_WEAPONS 4

typedef struct {
    Weapon weapons[MAX_WEAPONS];
    int activeWeaponCount;
} WeaponComponent;

void WeaponComponent_Init(WeaponComponent* comp);
void WeaponComponent_UpgradeOrAdd(WeaponComponent* comp, WeaponType type);
bool WeaponComponent_HasWeapon(WeaponComponent* comp, WeaponType type);

#endif // WEAPON_COMPONENT_H
