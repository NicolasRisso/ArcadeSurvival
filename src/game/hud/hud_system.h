#ifndef HUD_SYSTEM_H
#define HUD_SYSTEM_H

#include "game/core/player_state.h"

void HUDSystem_Draw(PlayerState* state);
void HUDSystem_DrawLevelUp(PlayerState* state);
void HUDSystem_DrawInventoryOverlay(PlayerState* state);

#endif // HUD_SYSTEM_H
