#include "Core/GameLoop.h"
#include "Framework_OOS/Actor.h"
#include "Graphics/SpriteComponent.h"

// --- Global variables for Act 2 test ---
Actor testActor;
SpriteComponent testSprite;
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
    
    // ACT 2: Initialize Test Actor
    Vector2 spawnPoint = { screenWidth / 2.0f, screenHeight / 2.0f };
    Actor_Init(&testActor, 1, spawnPoint);
    
    // Initialize its Sprite Component
    SpriteComponent_Init(&testSprite, &testActor, RED, 50.0f, 50.0f);
    
    // Attach Component to Actor
    // Important: We cast SpriteComponent to Component (this is how C inheritance works)
    Actor_AddComponent(&testActor, (Component*)&testSprite);
}

void ProcessInput(void)
{
    // TODO: Capture keyboard/gamepad inputs inside a PlayerController
    
    // As a placeholder, if we press ESC, raylib automatically flags the window to close
}

void UpdateLogic(float deltaTime)
{
    // Make the test actor spin slowly to test Component Updates implicitly
    testActor.rotation += 45.0f * deltaTime;
    
    // Update the actor and its components
    Actor_Update(&testActor, deltaTime);
}

void RenderGraphics(void)
{
    BeginDrawing();
    
    ClearBackground(RAYWHITE);

    // Dummy text to ensure the loop is running correctly
    DrawText("Act 2: OOS Framework is Running! (Spinning square is an Actor)", 100, 100, 40, DARKGREEN);
    DrawText("Press ESC to exit.", 100, 150, 20, DARKGRAY);

    // ACT 2: Render the OOS Actor (which invokes its SpriteComponent render)
    Actor_Render(&testActor);
    
    EndDrawing();
}


void CloseGame(void)
{
    // TODO: Unload textures, sounds, and clean up resources
    
    // Close Audio device
    CloseAudioDevice();
    
    // Close Window and OpenGL context
    CloseWindow();
}

bool IsGameReadyToClose(void)
{
    return WindowShouldClose();
}
