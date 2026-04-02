#include "Core/GameLoop.h"

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
    
    // TODO: Init other subsystems, allocate memory pools, load resources here
}

void ProcessInput(void)
{
    // TODO: Capture keyboard/gamepad inputs inside a PlayerController
    
    // As a placeholder, if we press ESC, raylib automatically flags the window to close
}

void UpdateLogic(float deltaTime)
{
    // TODO: Run OOS (Player) updates and ECS (Swarm) updates
}

void RenderGraphics(void)
{
    BeginDrawing();
    
    ClearBackground(RAYWHITE);

    // Dummy text to ensure the loop is running correctly
    DrawText("Act 1: Separated Game Loop is Running!", 100, 100, 40, DARKGREEN);
    DrawText("Press ESC to exit.", 100, 150, 20, DARKGRAY);

    // TODO: Render OOS elements (Player) 
    // TODO: Render ECS elements (Swarm)
    // TODO: Render UI
    
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
