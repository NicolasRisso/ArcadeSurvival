#ifndef ACTOR_H
#define ACTOR_H

#include "raylib.h"
#include "framework_oos/component.h"
#include <stdbool.h>

#define MAX_COMPONENTS_PER_ACTOR 8

typedef struct Actor {
    int id;
    bool bIsActive;
    
    // Core Transform data (I've put it here directly for performance instead of a TransformComponent)
    Vector2 position;
    float rotation;
    float scale;
    
    // Components array
    Component* components[MAX_COMPONENTS_PER_ACTOR];
    int componentCount;
} Actor;

// Initialize an Actor with default values
void Actor_Init(Actor* actor, int id, Vector2 spawnPos);

// Attach a component to this actor. The actor will subsequently update and render it.
bool Actor_AddComponent(Actor* actor, Component* comp);

// Update this actor and all of its components
void Actor_Update(Actor* actor, float deltaTime);

// Render this actor and all of its components
void Actor_Render(Actor* actor);

// Cleanup components if necessary
void Actor_Destroy(Actor* actor);

#endif // ACTOR_H
