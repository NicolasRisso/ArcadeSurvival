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
#include "game/swarm/projectile_system.h"
#include "game/swarm/pickup_system.h"
#include "game/hud/hud_system.h"

// --- Global variables for Testing ---
static PlayerController playerController;
static PlayerState playerState;
static PlayerCharacter playerCharacter;
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
    
    // Initialize Player Architecture
    Vector2 spawnPoint = { screenWidth / 2.0f, screenHeight / 2.0f };
    
    PlayerController_Init(&playerController);
    PlayerState_Init(&playerState);
    
    // Note: The PlayerCharacter aggregates the controller and state 
    PlayerCharacter_Init(&playerCharacter, 1, spawnPoint, &playerController, &playerState);

    // Initialize ECS and spawn initial swarm tests
    ECS_Init();
    EnemySystem_Init(spawnPoint);
}

void ProcessInput(void)
{
    // Capture Keyboard/Gamepad inputs into the Controller
    PlayerController_Update(&playerController);
}

void UpdateLogic(float deltaTime)
{
    // Update the Hero Character
    PlayerCharacter_Update(&playerCharacter, deltaTime);

    // Update Swarm behaviors
    Vector2 playerPos = playerCharacter.base.position;
    EnemySystem_Update(deltaTime, playerPos, &playerState);
    ProjectileSystem_Update(deltaTime);
    PickupSystem_Update(deltaTime, &playerState, playerPos);
}

void RenderGraphics(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Setup Camera2D
    Camera2D camera = { 0 };
    camera.target = playerCharacter.base.position;
    camera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    BeginMode2D(camera);

    // Draw Map Borders
    DrawRectangleLines(-5000, -5000, 10000, 10000, DARKGRAY);

    // Render Back Picking Items
    PickupSystem_DrawBackground();

    // Render the PlayerCharacter
    Actor_Render(&playerCharacter.base);
    
    // Render Swarm 
    SwarmRendererSystem_Draw();
    
    // Render Fore Picking items (magnetized ones)
    PickupSystem_DrawForeground();
    
    EndMode2D();
    
    HUDSystem_Draw(&playerState);
    
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
