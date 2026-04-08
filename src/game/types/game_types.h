#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include "raylib.h"

// --- Generic Utility Range Types ---
typedef struct {
    float min;
    float max;
} FloatRange;

typedef struct {
    int min;
    int max;
} IntRange;

// --- Core Game Enums ---
typedef enum {
    ENEMY_BASIC,
    ENEMY_FAST,
    ENEMY_TANK,
    ENEMY_MINI_BOSS
} EnemyType;

#endif // GAME_TYPES_H
