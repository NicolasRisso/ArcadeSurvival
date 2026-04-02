#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "framework_oos/actor.h"
#include "game/player_controller.h"
#include "game/player_state.h"
#include "graphics/sprite_component.h"

// PlayerCharacter inherits from Actor by having it as the first member
typedef struct {
    Actor base;
    
    // Components
    SpriteComponent spriteComp;
    
    // Systems
    PlayerController* controller;
    PlayerState* state;
    
    // Character Stats
    float moveSpeed;
} PlayerCharacter;

void PlayerCharacter_Init(PlayerCharacter* character, int id, Vector2 spawnPos, PlayerController* controller, PlayerState* state);
void PlayerCharacter_Update(PlayerCharacter* character, float deltaTime);

#endif // PLAYER_CHARACTER_H
