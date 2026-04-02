#include "game/player_character.h"

void PlayerCharacter_Init(PlayerCharacter* character, int id, Vector2 spawnPos, PlayerController* controller, PlayerState* state) {
    if (!character) return;

    // 1. Init Base Actor
    Actor_Init(&character->base, id, spawnPos);
    
    // 2. Link Systems
    character->controller = controller;
    character->state = state;
    character->moveSpeed = 300.0f; // Pixels per second
    
    // 3. Init and Attach Components
    // Give the player a blue square for now
    SpriteComponent_Init(&character->spriteComp, &character->base, BLUE, 40.0f, 40.0f);
    Actor_AddComponent(&character->base, (Component*)&character->spriteComp);
}

void PlayerCharacter_Update(PlayerCharacter* character, float deltaTime) {
    if (!character || !character->base.bIsActive || !character->state || character->state->bIsDead) return;

    // Read Input from Controller and Apply Movement
    if (character->controller) {
        // moveInput is already normalized
        Vector2 velocity = { 
            character->controller->moveInput.x * character->moveSpeed * deltaTime,
            character->controller->moveInput.y * character->moveSpeed * deltaTime
        };
        
        character->base.position.x += velocity.x;
        character->base.position.y += velocity.y;
    }
    
    // Call base Actor update to process any components attached
    Actor_Update(&character->base, deltaTime);
}
