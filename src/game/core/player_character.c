#include "game/core/player_character.h"
#include "game/systems/combat/combat_system.h"

void PlayerCharacter_Init(PlayerCharacter* character, int id, Vector2 spawnPos, PlayerController* controller, PlayerState* state) {
    if (!character) return;

    // 1. Init Base Actor
    Actor_Init(&character->base, id, spawnPos);
    
    // 2. Link Systems
    character->controller = controller;
    character->state = state;
    
    // 3. Init and Attach Components
    // Giving the player a blue square for now
    SpriteComponent_Init(&character->spriteComp, &character->base, BLUE, 40.0f, 40.0f);
    Actor_AddComponent(&character->base, (Component*)&character->spriteComp);

    // Initializing movement component
    MovementComponent_Init(&character->movementComp, &character->base, controller, 300.0f);
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
    
    // Clamp to map borders (-5000, -5000) to (5000, 5000)
    if (character->base.position.x < -5000.0f) character->base.position.x = -5000.0f;
    if (character->base.position.x > 5000.0f) character->base.position.x = 5000.0f;
    if (character->base.position.y < -5000.0f) character->base.position.y = -5000.0f;
    if (character->base.position.y > 5000.0f) character->base.position.y = 5000.0f;
    
    // Handle Visual feedback
    if (character->state->damageFlashTimer > 0.0f) {
        character->spriteComp.tint = WHITE;
    } else {
        character->spriteComp.tint = BLUE;
    }
    
    // Handle combat (auto-firing)
    CombatSystem_Update(deltaTime, character->state, character->base.position);
}
