#include "game/systems/combat/weapon_component.h"

void WeaponComponent_Init(WeaponComponent* comp) {
    if (!comp) return;
    comp->activeWeaponCount = 0;
    for (int i = 0; i < MAX_WEAPONS; i++) {
        comp->weapons[i].type = WEAPON_NONE;
    }
}

void WeaponComponent_UpgradeOrAdd(WeaponComponent* comp, WeaponType type) {
    if (!comp) return;

    // Check if we already have it
    for (int i = 0; i < comp->activeWeaponCount; i++) {
        if (comp->weapons[i].type == type) {
            if (comp->weapons[i].level < MAX_WEAPON_LEVEL) {
                comp->weapons[i].level++;
            }
            return;
        }
    }

    // New weapon
    if (comp->activeWeaponCount < MAX_WEAPONS) {
        Weapon newWp = {0};
        newWp.type = type;
        newWp.level = 1;
        comp->weapons[comp->activeWeaponCount] = newWp;
        comp->activeWeaponCount++;
    }
}

bool WeaponComponent_HasWeapon(WeaponComponent* comp, WeaponType type) {
    if (!comp) return false;
    for (int i = 0; i < comp->activeWeaponCount; i++) {
        if (comp->weapons[i].type == type) return true;
    }
    return false;
}
