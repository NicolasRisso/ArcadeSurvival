#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdbool.h>

// Forward declaration of Actor so Component can reference its owner
typedef struct Actor Actor;

// Enum for identifying component types (useful for ECS bridges)
typedef enum {
    COMP_BASE,
    COMP_SPRITE,
    COMP_MOVEMENT,
    COMP_COLLISION
} ComponentType;

// Base Component Structure
typedef struct Component Component;

// Function pointers for virtual-like behavior
typedef void (*ComponentUpdateFunc)(Component* self, float deltaTime);
typedef void (*ComponentRenderFunc)(Component* self);
typedef void (*ComponentDestroyFunc)(Component* self);

struct Component {
    ComponentType type;
    Actor* owner;
    bool bIsActive;

    // Virtual Functions
    ComponentUpdateFunc Update;
    ComponentRenderFunc Render;
    ComponentDestroyFunc Destroy;
};

// Base Initialization
void Component_Init(Component* comp, ComponentType type, Actor* owner);

// Invokes the virtual update if it exists
void Component_Update(Component* comp, float deltaTime);

// Invokes the virtual render if it exists
void Component_Render(Component* comp);

#endif // COMPONENT_H
