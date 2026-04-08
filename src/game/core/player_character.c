#include "game/core/player_character.h"
#include "game/systems/combat/combat_system.h"
#include "graphics/resource_manager.h"

void PlayerCharacter_Init(PlayerCharacter* character, int id, Vector2 spawnPos, PlayerController* controller, PlayerState* state) {
    if (!character) return;

    // 1. Init Base Actor
    Actor_Init(&character->base, id, spawnPos);
    
    // 2. Link Systems
    character->controller = controller;
    character->state = state;
    
    // 3. Init and Attach Components
    SpriteComponent_Init(&character->spriteComp, &character->base, WHITE, 48.0f, 48.0f);
    
    // Load Player Texture
    character->spriteComp.texture = Resources_GetTexture(TEX_PLAYER);
    if (character->spriteComp.texture.id > 0) {
        // Player has a 3-frame walk cycle
        SpriteComponent_SetAnimation(&character->spriteComp, 3, 8.0f);
    }

    Actor_AddComponent(&character->base, (Component*)&character->spriteComp);

    // Initializing movement component
    MovementComponent_Init(&character->movementComp, &character->base, controller, state, 330.0f);
    Actor_AddComponent(&character->base, (Component*)&character->movementComp);
}

void PlayerCharacter_Update(PlayerCharacter* character, float deltaTime) {
    // Note we reference state->health.bIsDead now
    if (!character || !character->base.bIsActive || !character->state || character->state->health.bIsDead) return;

    // Movement Component handles updating positional input by itself.
    
    // Handle state update (for flash timer decrement, health checks, etc)
    PlayerState_Update(character->state, deltaTime);
    
    // Call base Actor update to process any components attached
    Actor_Update(&character->base, deltaTime);

    // Set Sprite Flip based on movement direction
    if (character->controller->moveInput.x < 0) {
        character->spriteComp.flipX = true;
    } else if (character->controller->moveInput.x > 0) {
        character->spriteComp.flipX = false;
    }
    
    // Clamp to map borders (-5000, -5000) to (5000, 5000)
    if (character->base.position.x < -5000.0f) character->base.position.x = -5000.0f;
    if (character->base.position.x > 5000.0f) character->base.position.x = 5000.0f;
    if (character->base.position.y < -5000.0f) character->base.position.y = -5000.0f;
    if (character->base.position.y > 5000.0f) character->base.position.y = 5000.0f;
    
    // Handle Visual feedback (Flash/Invulnerability)
    if (character->state->damageFlashTimer > 0.0f) {
        // Oscillate every 0.1s (0.05s on, 0.05s off)
        if (((int)(character->state->damageFlashTimer * 20)) % 2 == 0) {
            character->spriteComp.tint = RED; // Flash Red
        } else {
            character->spriteComp.tint = WHITE;
        }
    } else {
        character->spriteComp.tint = WHITE; // Normal colors
    }
    
    // Handle combat (auto-firing)
    CombatSystem_Update(deltaTime, character->state, character->base.position);
}
