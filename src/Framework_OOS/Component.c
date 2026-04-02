#include "Framework_OOS/Component.h"
#include <stddef.h>

void Component_Init(Component* comp, ComponentType type, Actor* owner)
{
    if (!comp) return;

    comp->type = type;
    comp->owner = owner;
    comp->bIsActive = true;

    // Default to NULL (No-op)
    comp->Update = NULL;
    comp->Render = NULL;
    comp->Destroy = NULL;
}

void Component_Update(Component* comp, float deltaTime)
{
    if (comp && comp->bIsActive && comp->Update)
    {
        comp->Update(comp, deltaTime);
    }
}

void Component_Render(Component* comp)
{
    if (comp && comp->bIsActive && comp->Render)
    {
        comp->Render(comp);
    }
}
