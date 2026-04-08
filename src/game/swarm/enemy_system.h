#ifndef ENEMY_SYSTEM_H
#define ENEMY_SYSTEM_H

#include "raylib.h"
#include "game/types/game_types.h"
#include "game/core/player_state.h"

// --- Spawning Specific Types ---
typedef enum {
    SPAWN_SINGLE,
    SPAWN_LINE,
    SPAWN_CLUSTER,
    SPAWN_SURROUND
} SpawnPattern;

typedef struct {
    SpawnPattern pattern;
    float weight;           // Relative chance (e.g., 20.0 is 2x 10.0)
    IntRange countRange;    // Range for how many enemies spawn
    FloatRange distRange;   // Distance range from the player
    EnemyType enemyType;    // Model identifier
    float difficulty;       // Minimum spawner difficulty to unlock this preset
} EnemySpawnPreset;

void EnemySystem_Init(Vector2 playerSpawnPos);
void EnemySystem_Update(float deltaTime, Vector2 playerPos, PlayerState* playerState);
void EnemySystem_TriggerPreset(Vector2 playerPos, EnemySpawnPreset preset, int minutes);

#endif // ENEMY_SYSTEM_H
