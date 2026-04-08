#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "raylib.h"

// Available texture keys
#define TEX_PLAYER "player"
#define TEX_ENEMIES "enemies"
#define TEX_PICKUPS "pickups"
#define TEX_MAP "map"

// Available audio keys
#define SND_EXPLOSION "explosion"
#define SND_XP_GAIN "xp_gain"
#define SND_LEVEL_UP "level_up"
#define SND_ENEMY_HIT "enemy_hit"
#define SND_PLAYER_HIT "player_hit"
#define MUS_COMBAT "combat"

void Resources_Init(void);
void Resources_Unload(void);
Texture2D Resources_GetTexture(const char* key);
Sound Resources_GetSound(const char* key);
Music Resources_GetMusic(const char* key);

#endif // RESOURCE_MANAGER_H
