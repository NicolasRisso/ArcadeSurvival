#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

#include "framework_oos/component.h"
#include "framework_oos/actor.h"
#include "game/core/player_controller.h"

// Forward declaration to avoid circular dependency if possible, 
// but since movement_component is simple enough we'll just use it.
struct PlayerState;

// MovementComponent inherits from Component
typedef struct {
    Component base;
    
    PlayerController* controller;
    struct PlayerState* state;
    float moveSpeed;
} MovementComponent;

void MovementComponent_Init(MovementComponent* movementComp, Actor* owner, PlayerController* controller, struct PlayerState* state, float moveSpeed);

#endif // MOVEMENT_COMPONENT_H
