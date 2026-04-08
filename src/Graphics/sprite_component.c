#include "graphics/sprite_component.h"
#include "framework_oos/actor.h"
#include <math.h>

// Virtual Update: Handle frame cycling and bobbing
static void SpriteComponent_Update(Component* self, float deltaTime)
{
    SpriteComponent* sprite = (SpriteComponent*)self;
    if (!sprite || !sprite->base.bIsActive) return;

    // 1. Handle Frame Animation
    if (sprite->frameCount > 1)
    {
        sprite->frameTimer += deltaTime;
        if (sprite->frameTimer >= sprite->frameTime)
        {
            sprite->frameTimer = 0.0f;
            sprite->currentFrame = (sprite->currentFrame + 1) % sprite->frameCount;
            
            // Note: This assumes horizontal layout
            sprite->sourceRect.x = sprite->currentFrame * sprite->sourceRect.width;
        }
    }

    // 2. Handle Bobbing
    if (sprite->bobSpeed > 0.0f)
    {
        sprite->bobTimer += deltaTime;
    }
}

// Virtual Render: Draw texture if loaded, otherwise fallback to rectangle
static void SpriteComponent_Render(Component* self)
{
    SpriteComponent* sprite = (SpriteComponent*)self;
    if (!sprite || !sprite->base.owner) return;
    
    Actor* owner = sprite->base.owner;
    
    // Calculate final position with bobbing offset
    float bobOffset = 0.0f;
    if (sprite->bobSpeed > 0.0f)
    {
        bobOffset = sinf(sprite->bobTimer * sprite->bobSpeed) * sprite->bobAmount;
    }

    Vector2 drawPos = { owner->position.x, owner->position.y + bobOffset };

    if (sprite->texture.id == 0)
    {
        // Fallback to geometric shape
        Rectangle rect = { 
            drawPos.x, 
            drawPos.y, 
            sprite->width * owner->scale, 
            sprite->height * owner->scale 
        };
        Vector2 origin = { rect.width / 2.0f, rect.height / 2.0f };
        DrawRectanglePro(rect, origin, owner->rotation, sprite->tint);
    }
    else
    {
        // Draw Texture
        Rectangle dest = { 
            drawPos.x, 
            drawPos.y, 
            sprite->width * owner->scale, 
            sprite->height * owner->scale 
        };
        Vector2 origin = { dest.width / 2.0f, dest.height / 2.0f };
        
        // Handle Horizontal Flip
        Rectangle source = sprite->sourceRect;
        if (sprite->flipX) source.width *= -1.0f;
        
        DrawTexturePro(sprite->texture, source, dest, origin, owner->rotation, sprite->tint);
    }
}

void SpriteComponent_Init(SpriteComponent* spriteComp, Actor* owner, Color tint, float w, float h)
{
    if (!spriteComp) return;

    // Initialize base
    Component_Init(&spriteComp->base, COMP_SPRITE, owner);
    
    spriteComp->texture.id = 0;
    spriteComp->tint = tint;
    spriteComp->width = w;
    spriteComp->height = h;
    
    // Animation defaults
    spriteComp->sourceRect = (Rectangle){ 0, 0, w, h }; // Default to full size
    spriteComp->frameCount = 1;
    spriteComp->currentFrame = 0;
    spriteComp->frameTimer = 0.0f;
    spriteComp->frameTime = 0.1f;
    
    // Bobbing defaults
    spriteComp->bobAmount = 0.0f;
    spriteComp->bobSpeed = 0.0f;
    spriteComp->bobTimer = (float)GetRandomValue(0, 1000) / 100.0f; // Random start phase

    spriteComp->flipX = false;

    // Overrides
    spriteComp->base.Update = SpriteComponent_Update;
    spriteComp->base.Render = SpriteComponent_Render;
}

void SpriteComponent_SetAnimation(SpriteComponent* spriteComp, int frameCount, float fps)
{
    if (!spriteComp) return;
    spriteComp->frameCount = frameCount;
    spriteComp->frameTime = 1.0f / fps;
    // Adjust source rect width if it's a spritesheet
    if (spriteComp->texture.id > 0)
    {
        spriteComp->sourceRect.width = (float)spriteComp->texture.width / frameCount;
        spriteComp->sourceRect.height = (float)spriteComp->texture.height;
    }
}
