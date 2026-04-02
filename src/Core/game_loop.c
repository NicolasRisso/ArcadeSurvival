#include "core/game_loop.h"
#include "framework_oos/actor.h"
#include "graphics/sprite_component.h"
#include "game/core/player_controller.h"
#include "game/core/player_state.h"
#include "game/core/player_character.h"
#include <stdio.h>

#include "framework_ecs/ecs_core.h"
#include "game/swarm/swarm_renderer_system.h"
#include "game/swarm/enemy_system.h"

// --- Global variables for Act 3 ---
static PlayerController playerController;
static PlayerState playerState;
static PlayerCharacter hero;
// ---------------------------------------

void InitGame(void)
{
    // Define window dimensions based on requirements
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    // Optional flags from suggestions (MSAA and window style)
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_UNDECORATED);
    
    // Initialize Raylib window
    InitWindow(screenWidth, screenHeight, "Arcade Survival Prototype");

    // Target a high frame rate capability
    SetTargetFPS(240);
    
    // Hide default OS cursor
    DisableCursor();

    // Initialize Audio device
    InitAudioDevice();
    
    // ACT 3: Initialize Player Architecture
    Vector2 spawnPoint = { screenWidth / 2.0f, screenHeight / 2.0f };
    
    PlayerController_Init(&playerController);
    PlayerState_Init(&playerState);
    
    // Note: The PlayerCharacter aggregates the controller and state 
    PlayerCharacter_Init(&hero, 1, spawnPoint, &playerController, &playerState);

    // ACT 4: Initialize ECS and spawn initial swarm tests
    ECS_Init();
    EnemySystem_Init(spawnPoint);
}

void ProcessInput(void)
{
    // ACT 3: Capture Keyboard/Gamepad inputs into the Controller
    PlayerController_Update(&playerController);
}

void UpdateLogic(float deltaTime)
{
    // ACT 3: Update the Hero Character
    PlayerCharacter_Update(&hero, deltaTime);

    // ACT 4: Update Swarm behaviors
    Vector2 playerPos = hero.base.position;
    EnemySystem_Update(deltaTime, playerPos);
}

void RenderGraphics(void)
{
    BeginDrawing();
    
    ClearBackground(RAYWHITE);

    // Dummy text to ensure the loop is running correctly
    DrawText("Act 3: Player Architecture Running! (Use WASD to Move)", 100, 100, 40, DARKGREEN);
    DrawText("Press ESC to exit.", 100, 150, 20, DARKGRAY);

    // Render Stats for debugging
    char statsText[128];
    sprintf(statsText, "HP: %d/%d  LVL: %d  EXP: %d", 
        playerState.health.currentHealth, playerState.health.maxHealth, playerState.level, playerState.experience);
    DrawText(statsText, 100, 200, 30, BLUE);

    // ACT 3: Render the Hero (Renders base Actor -> SpriteComponent)
    Actor_Render(&hero.base);
    
    // ACT 4: Render Swarm using the direct data array loop
    SwarmRendererSystem_Draw();
    
    EndDrawing();
}

void CloseGame(void)
{
    // Close Audio device
    CloseAudioDevice();
    
    // Close Window and OpenGL context
    CloseWindow();
}

bool IsGameReadyToClose(void)
{
    return WindowShouldClose();
}
