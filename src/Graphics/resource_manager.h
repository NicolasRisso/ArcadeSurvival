#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "raylib.h"

// Available texture keys
#define TEX_PLAYER "player"
#define TEX_ENEMIES "enemies"
#define TEX_PICKUPS "pickups"
#define TEX_MAP "map"

void Resources_Init(void);
void Resources_Unload(void);
Texture2D Resources_GetTexture(const char* key);

#endif // RESOURCE_MANAGER_H
