#include "game/core/player_controller.h"
#include "raymath.h"

void PlayerController_Init(PlayerController* controller) {
    if (!controller) return;
    controller->moveInput = (Vector2){ 0.0f, 0.0f };
    controller->bIsUsingGamepad = false;
}

void PlayerController_Update(PlayerController* controller) {
    if (!controller) return;

    controller->moveInput = (Vector2){ 0.0f, 0.0f };

    // Detect Input Mode Switch
    // 1. Check for Mouse/KB activity
    Vector2 mouseDelta = GetMouseDelta();
    if (fabsf(mouseDelta.x) > 0.1f || fabsf(mouseDelta.y) > 0.1f || 
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || GetKeyPressed() != 0) {
        controller->bIsUsingGamepad = false;
    }

    // Keyboard support
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    controller->moveInput.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  controller->moveInput.y += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  controller->moveInput.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) controller->moveInput.x += 1.0f;

    // Gamepad support (Left Thumbstick)
    if (IsGamepadAvailable(0)) {
        float stickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        float stickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
        
        // Deadzone check
        if (fabsf(stickX) > 0.2f || fabsf(stickY) > 0.2f) {
            controller->moveInput.x += stickX;
            controller->moveInput.y += stickY;
            controller->bIsUsingGamepad = true;
        }

        // Check for any gamepad button press to switch mode
        for (int i = 0; i < 15; i++) {
            if (IsGamepadButtonPressed(0, i)) {
                controller->bIsUsingGamepad = true;
                break;
            }
        }
    }

    // Normalize safely to avoid faster diagonal movement
    controller->moveInput = Vector2Normalize(controller->moveInput);
}
