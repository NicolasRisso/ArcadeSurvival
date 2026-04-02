#include "game/player/movement/movement_component.h"

// Internal virtual function override mapping
static void MovementComponent_Update(Component* self, float deltaTime) {
    if (!self || !self->owner) return;
    
    MovementComponent* movementComp = (MovementComponent*)self;
    
    if (movementComp->controller) {
        Vector2 velocity = { 
            movementComp->controller->moveInput.x * movementComp->moveSpeed * deltaTime,
            movementComp->controller->moveInput.y * movementComp->moveSpeed * deltaTime
        };
        
        self->owner->position.x += velocity.x;
        self->owner->position.y += velocity.y;
    }
}

void MovementComponent_Init(MovementComponent* movementComp, Actor* owner, PlayerController* controller, float moveSpeed) {
    if (!movementComp || !owner) return;
    
    // Initialize base Component (Assuming COMP_MOVEMENT is available in enum)
    Component_Init(&movementComp->base, COMP_MOVEMENT, owner);
    
    // Override the Update function specifically for this component type
    movementComp->base.Update = MovementComponent_Update;
    
    // Setup specific MovementComponent variables
    movementComp->controller = controller;
    movementComp->moveSpeed = moveSpeed;
}
