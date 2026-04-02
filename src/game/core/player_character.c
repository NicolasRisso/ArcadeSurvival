#include "game/core/player_character.h"

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
    
    // Call base Actor update to process any components attached
    Actor_Update(&character->base, deltaTime);
}
