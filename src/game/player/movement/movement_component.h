#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

#include "framework_oos/component.h"
#include "framework_oos/actor.h"
#include "game/core/player_controller.h"

// MovementComponent inherits from Component
typedef struct {
    Component base;
    
    PlayerController* controller;
    float moveSpeed;
} MovementComponent;

void MovementComponent_Init(MovementComponent* movementComp, Actor* owner, PlayerController* controller, float moveSpeed);

#endif // MOVEMENT_COMPONENT_H
