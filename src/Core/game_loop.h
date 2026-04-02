#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "raylib.h"

// Initializes the game, Raylib, and all subsystems
void InitGame(void);

// Processes player input and general hardware inputs
void ProcessInput(void);

// Updates the game logic (movement, physics, AI) based on delta time
void UpdateLogic(float deltaTime);

// Renders all graphics to the screen
void RenderGraphics(void);

// Cleans up resources before closing
void CloseGame(void);

// Returns true if the game window should close (e.g. pressing ESC or X)
bool IsGameReadyToClose(void);

#endif // GAMELOOP_H
