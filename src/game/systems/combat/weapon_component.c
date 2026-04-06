#include "game/systems/combat/weapon_component.h"

void WeaponComponent_Init(WeaponComponent* comp) {
    if (!comp) return;
    comp->activeWeaponCount = 0;
    for (int i = 0; i < MAX_WEAPONS; i++) {
        comp->weapons[i].type = WEAPON_NONE;
    }
}

void WeaponComponent_AddWeapon(WeaponComponent* comp, Weapon weapon) {
    if (!comp) return;
    if (comp->activeWeaponCount < MAX_WEAPONS) {
        comp->weapons[comp->activeWeaponCount] = weapon;
        comp->activeWeaponCount++;
    }
}
