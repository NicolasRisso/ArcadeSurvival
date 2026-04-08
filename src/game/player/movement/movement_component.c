#include "game/player/movement/movement_component.h"
#include "game/core/player_state.h"

// Internal virtual function override mapping
static void MovementComponent_Update(Component* self, float deltaTime) {
    if (!self || !self->owner) return;
    
    MovementComponent* movementComp = (MovementComponent*)self;
    
    if (movementComp->controller) {
        float speed = movementComp->moveSpeed;
        if (movementComp->state) {
            speed *= movementComp->state->stats.movementMultiplier;
        }

        Vector2 velocity = { 
            movementComp->controller->moveInput.x * speed * deltaTime,
            movementComp->controller->moveInput.y * speed * deltaTime
        };
        
        self->owner->position.x += velocity.x;
        self->owner->position.y += velocity.y;
    }
}

void MovementComponent_Init(MovementComponent* movementComp, Actor* owner, PlayerController* controller, PlayerState* state, float moveSpeed) {
    if (!movementComp || !owner) return;
    
    // Initialize base Component (Assuming COMP_MOVEMENT is available in enum)
    Component_Init(&movementComp->base, COMP_MOVEMENT, owner);
    
    // Override the Update function specifically for this component type
    movementComp->base.Update = MovementComponent_Update;
    
    // Setup specific MovementComponent variables
    movementComp->controller = controller;
    movementComp->state = state;
    movementComp->moveSpeed = moveSpeed;
}
