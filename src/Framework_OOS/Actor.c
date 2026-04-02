#include "Framework_OOS/Actor.h"
#include <stddef.h>

void Actor_Init(Actor* actor, int id, Vector2 spawnPos)
{
    if (!actor) return;

    actor->id = id;
    actor->bIsActive = true;
    actor->position = spawnPos;
    actor->rotation = 0.0f;
    actor->scale = 1.0f;
    actor->componentCount = 0;

    for (int i = 0; i < MAX_COMPONENTS_PER_ACTOR; i++)
    {
        actor->components[i] = NULL;
    }
}

bool Actor_AddComponent(Actor* actor, Component* comp)
{
    if (!actor || !comp) return false;
    
    if (actor->componentCount < MAX_COMPONENTS_PER_ACTOR)
    {
        actor->components[actor->componentCount] = comp;
        actor->componentCount++;
        return true;
    }
    
    return false; // Out of bounds
}

void Actor_Update(Actor* actor, float deltaTime)
{
    if (!actor || !actor->bIsActive) return;

    for (int i = 0; i < actor->componentCount; i++)
    {
        Component_Update(actor->components[i], deltaTime);
    }
}

void Actor_Render(Actor* actor)
{
    if (!actor || !actor->bIsActive) return;

    for (int i = 0; i < actor->componentCount; i++)
    {
        Component_Render(actor->components[i]);
    }
}

void Actor_Destroy(Actor* actor)
{
    if (!actor) return;

    for (int i = 0; i < actor->componentCount; i++)
    {
        if (actor->components[i] && actor->components[i]->Destroy)
        {
            actor->components[i]->Destroy(actor->components[i]);
        }
    }
    actor->bIsActive = false;
}
