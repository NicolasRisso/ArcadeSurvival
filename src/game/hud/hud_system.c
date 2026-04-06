#include "game/hud/hud_system.h"
#include "raylib.h"
#include <stdio.h>

void HUDSystem_Draw(PlayerState* state) {
    if (!state) return;
    
    int screenWidth = GetScreenWidth();
    int barHeight = 20;
    
    float xpRatio = (float)state->xp.currentXP / (float)state->xp.xpToNextLevel;
    if (xpRatio > 1.0f) xpRatio = 1.0f;
    
    // Background XP bar
    DrawRectangle(0, 0, screenWidth, barHeight, DARKGRAY);
    // Filled XP bar
    DrawRectangle(0, 0, (int)(screenWidth * xpRatio), barHeight, BLUE);
    
    // XP Text
    char levelText[32];
    sprintf(levelText, "Level %d", state->xp.currentLevel);
    int fontSize = 20;
    int textWidth = MeasureText(levelText, fontSize);
    DrawText(levelText, (screenWidth - textWidth) / 2, 0, fontSize, WHITE);
    
    // HEALTH BAR
    int hpBarWidth = (int)(screenWidth * 0.30f);
    int hpBarX = 10;
    int hpBarY = barHeight + 5;
    
    float hpRatio = (float)state->health.currentHealth / (float)state->health.maxHealth;
    if (hpRatio < 0.0f) hpRatio = 0.0f;
    
    DrawRectangle(hpBarX, hpBarY, hpBarWidth, barHeight, DARKGRAY);
    DrawRectangle(hpBarX, hpBarY, (int)(hpBarWidth * hpRatio), barHeight, RED);
    
    char hpText[32];
    sprintf(hpText, "%d/%d", state->health.currentHealth, state->health.maxHealth);
    int hpTextWidth = MeasureText(hpText, fontSize);
    DrawText(hpText, hpBarX + (hpBarWidth - hpTextWidth) / 2, hpBarY, fontSize, WHITE);
    
    // Also draw active powerups if they exist
    extern float g_TimeFreezeTimer;
    extern float g_DoubleTroubleTimer;
    
    int yOffset = barHeight + 5;
    if (g_TimeFreezeTimer > 0) {
        char buff[64];
        sprintf(buff, "TIME FREEZE: %.1f", g_TimeFreezeTimer);
        int textWidth = MeasureText(buff, 20);
        DrawText(buff, (screenWidth - textWidth) / 2, yOffset, 20, SKYBLUE);
        yOffset += 25;
    }
    
    if (g_DoubleTroubleTimer > 0) {
        char buff[64];
        sprintf(buff, "DOUBLE TROUBLE: %.1f", g_DoubleTroubleTimer);
        int textWidth = MeasureText(buff, 20);
        DrawText(buff, (screenWidth - textWidth) / 2, yOffset, 20, ORANGE);
        yOffset += 25;
    }
}
