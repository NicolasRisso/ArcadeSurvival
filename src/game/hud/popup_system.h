#ifndef POPUP_SYSTEM_H
#define POPUP_SYSTEM_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_DAMAGE_POPUPS 256

typedef struct {
    Vector2 position;
    int amount;
    float timer;
    bool active;
} DamagePopup;

void PopupSystem_Init(void);
void PopupSystem_Add(Vector2 position, int amount);
void PopupSystem_Update(float deltaTime);
void PopupSystem_Draw(void);

#endif // POPUP_SYSTEM_H
