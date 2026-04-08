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
    
    // Animation Support
    Rectangle sourceRect;
    int frameCount;
    int currentFrame;
    float frameTimer;
    float frameTime;
    
    // Automatic Bobbing (for bats etc)
    float bobAmount;
    float bobSpeed;
    float bobTimer;
    
    // Rendering flags
    bool flipX;
} SpriteComponent;

// Initializes the SpriteComponent. 
void SpriteComponent_Init(SpriteComponent* spriteComp, Actor* owner, Color tint, float w, float h);

// Sets up animation frames (assumes horizontal spritesheet)
void SpriteComponent_SetAnimation(SpriteComponent* spriteComp, int frameCount, float fps);

#endif // SPRITE_COMPONENT_H
