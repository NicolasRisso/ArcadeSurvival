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
    EnemySystem_Update(deltaTime, playerPos);
    ProjectileSystem_Update(deltaTime);
    PickupSystem_Update(deltaTime, &playerState, playerPos);
}

void RenderGraphics(void)
{
    BeginDrawing();
    
    ClearBackground(RAYWHITE);

    // Dummy text to ensure the loop is running correctly
    DrawText("ArcadeSurvivors", 100, 100, 40, DARKGREEN);
    DrawText("Press ESC to exit.", 100, 150, 20, DARKGRAY);

    // Render the PlayerCharacter (Renders base Actor -> SpriteComponent)
    Actor_Render(&playerCharacter.base);
    
    // Render Swarm using the direct data array loop
    SwarmRendererSystem_Draw();
    PickupSystem_Draw();
    
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
