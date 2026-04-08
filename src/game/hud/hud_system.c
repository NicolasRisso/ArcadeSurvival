#include "game/hud/hud_system.h"
#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void HUDSystem_Draw(PlayerState* state) {
    if (!state) return;
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
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
    
    // GAME TIMER
    int minutes = (int)(state->gameTime / 60);
    int seconds = (int)(state->gameTime) % 60;
    char timeBuf[16];
    sprintf(timeBuf, "%02d:%02d", minutes, seconds);
    int timeTextWidth = MeasureText(timeBuf, 25);
    int timerX = screenWidth - timeTextWidth - 20;
    int timerY = barHeight + 35;
    DrawText(timeBuf, timerX, timerY, 25, BLACK);

    // STAT NOTIFICATION LOG
    if (state->statNotify.timer > 0) {
        DrawText(state->statNotify.message, screenWidth - MeasureText(state->statNotify.message, 20) - 20, timerY + 35, 20, DARKGREEN);
    }
    
    // SWARM WARNING
    float swarmCycle = fmodf(state->gameTime, 120.0f);
    if (state->gameTime > 110.0f && swarmCycle >= 115.0f && swarmCycle < 120.0f) {
        const char* warning = "SWARM INCOMING";
        int fontSize = 50;
        int tWidth = MeasureText(warning, fontSize);
        
        // Flickering effect
        float alpha = (sinf(state->gameTime * 12.0f) * 0.5f + 0.5f);
        Color warnCol = (Color){ 255, 0, 0, (unsigned char)(alpha * 255) };
        
        DrawText(warning, (screenWidth - tWidth) / 2, screenHeight / 2 - 50, fontSize, warnCol);
    }
}

#include "game/systems/combat/weapon_data.h"
#include "game/systems/relics/relic_data.h"

static void DrawTextWrapped(const char* text, Vector2 pos, float maxWidth, int fontSize, Color color) {
    float spacing = 2.0f;
    Font font = GetFontDefault();
    
    char buffer[512];
    strcpy(buffer, text);
    
    char* words[64];
    int wordCount = 0;
    
    char* word = strtok(buffer, " \n");
    while (word != NULL && wordCount < 64) {
        words[wordCount++] = word;
        word = strtok(NULL, " \n");
    }
    
    float cursorX = 0;
    float cursorY = 0;
    float spaceWidth = MeasureTextEx(font, " ", (float)fontSize, spacing).x;
    
    for (int i = 0; i < wordCount; i++) {
        Vector2 size = MeasureTextEx(font, words[i], (float)fontSize, spacing);
        
        if (cursorX + size.x > maxWidth && cursorX > 0) {
            cursorX = 0;
            cursorY += (float)fontSize + 2;
        }
        
        DrawTextEx(font, words[i], (Vector2){pos.x + cursorX, pos.y + cursorY}, (float)fontSize, spacing, color);
        cursorX += size.x + spaceWidth;
    }
}

static void DrawCard(Rectangle bounds, LevelUpOption option, bool isHovered) {
    DrawRectangleRec(bounds, isHovered ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(bounds, 3, isHovered ? YELLOW : BLACK);
    
    const char* name = "";
    char desc[512]; // Increased buffer slightly

    if (option.type == OPTION_WEAPON) {
        name = GetWeaponName(option.weaponType);
        GetWeaponDescription(option.weaponType, option.currentLevel, desc);
    } else {
        name = GetRelicName(option.relicType);
        GetRelicDescription(option.relicType, option.currentLevel, desc);
    }
    
    int titleSize = 25;
    DrawText(name, (int)bounds.x + 10, (int)bounds.y + 10, titleSize, BLACK);
    
    if (option.currentLevel == 0) {
        DrawText("NEW!", (int)bounds.x + 10, (int)bounds.y + 40, 20, MAROON);
    } else {
        char lvBuf[16];
        sprintf(lvBuf, "LEVEL %d", option.currentLevel + 1);
        DrawText(lvBuf, (int)bounds.x + 10, (int)bounds.y + 40, 20, DARKGREEN);
    }
    
    // Draw wrapped description
    DrawTextWrapped(desc, (Vector2){bounds.x + 10, bounds.y + 80}, bounds.width - 20, 18, DARKGRAY);
}

void HUDSystem_DrawLevelUp(PlayerState* state) {
    if (!state || !state->bIsLevelingUp) return;
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Dim background
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 150 });
    
    int cardWidth = 300;
    int cardHeight = 450;
    int spacing = 50;
    int totalWidth = (state->levelUpOptionCount * cardWidth) + ((state->levelUpOptionCount - 1) * spacing);
    int startX = (screenWidth - totalWidth) / 2;
    int startY = (screenHeight - cardHeight) / 2;
    
    Vector2 mousePos = GetMousePosition();
    
    const char* title = "LEVEL UP! CHOOSE AN UPGRADE";
    DrawText(title, (screenWidth - MeasureText(title, 40)) / 2, startY - 80, 40, WHITE);

    for (int i = 0; i < state->levelUpOptionCount; i++) {
        Rectangle bounds = { (float)startX + i * (cardWidth + spacing), (float)startY, (float)cardWidth, (float)cardHeight };
        bool isHovered = CheckCollisionPointRec(mousePos, bounds);
        DrawCard(bounds, state->levelUpOptions[i], isHovered);
        
        // Key hint
        char hint[4];
        sprintf(hint, "%d", i + 1);
        DrawText(hint, (int)bounds.x + cardWidth / 2 - 10, (int)bounds.y + cardHeight + 20, 30, WHITE);
    }
}

void HUDSystem_DrawInventoryOverlay(PlayerState* state) {
    if (!state || !state->bShowInventoryOverlay) return;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Dim background
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 200 });

    int fontSize = 28;
    int spacing = 40;
    int centerY = screenHeight / 2;

    // --- LEFT COLUMN (EQUIPMENT) ---
    int leftX = (int)(screenWidth * 0.15f);
    int y = centerY - 250;

    DrawText("EQUIPMENT", leftX, y, 36, GOLD);
    y += 60;

    // Weapons
    for (int i = 0; i < state->weapons.activeWeaponCount; i++) {
        char buf[128];
        sprintf(buf, "Lv%d %s", state->weapons.weapons[i].level, GetWeaponName(state->weapons.weapons[i].type));
        DrawText(buf, leftX, y, fontSize, WHITE);
        y += spacing;
    }

    y += 30; // gap

    // Relics
    for (int i = 0; i < state->relics.activeRelicCount; i++) {
        char buf[128];
        sprintf(buf, "Lv%d %s", state->relics.relics[i].level, GetRelicName(state->relics.relics[i].type));
        DrawText(buf, leftX, y, fontSize, SKYBLUE);
        y += spacing;
    }

    // --- RIGHT COLUMN (STATS) ---
    int rightX = (int)(screenWidth * 0.60f);
    y = centerY - 250;

    DrawText("PLAYER ATTRIBUTES", rightX, y, 36, GOLD);
    y += 60;

    char statText[128];

    // Health
    sprintf(statText, "Health: %d", state->health.maxHealth);
    DrawText(statText, rightX, y, fontSize, WHITE); y += spacing;

    // Damage
    sprintf(statText, "Damage: %.0f%%", state->stats.damageMultiplier * 100.0f);
    DrawText(statText, rightX, y, fontSize, WHITE); y += spacing;

    // Attack Speed
    sprintf(statText, "Attack Speed: %.0f%%", state->stats.attackSpeedMultiplier * 100.0f);
    DrawText(statText, rightX, y, fontSize, WHITE); y += spacing;

    // Size (Area)
    sprintf(statText, "Size: %.0f%%", state->stats.sizeMultiplier * 100.0f);
    DrawText(statText, rightX, y, fontSize, WHITE); y += spacing;

    // Movement
    sprintf(statText, "Movement: %.0f%%", state->stats.movementMultiplier * 100.0f);
    DrawText(statText, rightX, y, fontSize, WHITE); y += spacing;

    // LifeSteal
    sprintf(statText, "Life Steal: %.1f%%", state->stats.lifeSteal * 100.0f);
    DrawText(statText, rightX, y, fontSize, WHITE); y += spacing;

    const char* tabHint = "( Release TAB to close )";
    DrawText(tabHint, (screenWidth - MeasureText(tabHint, 20)) / 2, screenHeight - 50, 20, GRAY);
}
