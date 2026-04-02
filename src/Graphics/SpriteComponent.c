#include "Graphics/SpriteComponent.h"
#include "Framework_OOS/Actor.h"

// Internal virtual function overrides mapping
static void SpriteComponent_Render(Component* self)
{
    // Cast base component pointer to derived type
    SpriteComponent* sprite = (SpriteComponent*)self;
    
    // Safety check
    if (!sprite || !sprite->base.owner) return;
    
    Actor* owner = sprite->base.owner;
    
    // For now, if texture is not loaded, just draw a solid color rectangle centered on the Actor
    if (sprite->texture.id == 0)
    {
        Rectangle rect = { 
            owner->position.x, 
            owner->position.y, 
            sprite->width * owner->scale, 
            sprite->height * owner->scale 
        };
        Vector2 origin = { rect.width / 2.0f, rect.height / 2.0f };
        
        DrawRectanglePro(rect, origin, owner->rotation, sprite->tint);
    }
    else
    {
        // TODO: Draw actual texture with scaling and rotation
    }
}

void SpriteComponent_Init(SpriteComponent* spriteComp, Actor* owner, Color tint, float w, float h)
{
    if (!spriteComp) return;

    // Initialize base component first
    Component_Init(&spriteComp->base, COMP_SPRITE, owner);
    
    // Setup derived data
    spriteComp->texture.id = 0; // Empty texture flag
    spriteComp->tint = tint;
    spriteComp->width = w;
    spriteComp->height = h;

    // Override the virtual render function
    spriteComp->base.Render = SpriteComponent_Render;
}
