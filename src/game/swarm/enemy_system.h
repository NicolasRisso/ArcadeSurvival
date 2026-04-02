#ifndef ENEMY_SYSTEM_H
#define ENEMY_SYSTEM_H

#include "raylib.h"

void EnemySystem_Init(Vector2 playerSpawnPos);
void EnemySystem_Update(float deltaTime, Vector2 playerPos);

#endif // ENEMY_SYSTEM_H
