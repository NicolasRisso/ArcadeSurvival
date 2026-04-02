#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "framework_oos/component.h"
#include "raylib.h"

// SpriteComponent inherits from Component by having it as the first member
typedef struct {
    Component base; 
    Texture2D texture;
    Color tint;
    float width;
    float height;
} SpriteComponent;

// Initializes the SpriteComponent. 
void SpriteComponent_Init(SpriteComponent* spriteComp, Actor* owner, Color tint, float w, float h);

#endif // SPRITE_COMPONENT_H
