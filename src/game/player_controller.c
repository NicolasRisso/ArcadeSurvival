#include "game/player_controller.h"
#include "raymath.h"

void PlayerController_Init(PlayerController* controller) {
    if (!controller) return;
    controller->moveInput = (Vector2){ 0.0f, 0.0f };
}

void PlayerController_Update(PlayerController* controller) {
    if (!controller) return;

    controller->moveInput = (Vector2){ 0.0f, 0.0f };

    // Keyboard support
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    controller->moveInput.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  controller->moveInput.y += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  controller->moveInput.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) controller->moveInput.x += 1.0f;

    // Normalize safely to avoid faster diagonal movement
    controller->moveInput = Vector2Normalize(controller->moveInput);
    
    // Future Gamepad support can be added here
}
