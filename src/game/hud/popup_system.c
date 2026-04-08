#include "game/hud/popup_system.h"
#include <stdio.h>

static DamagePopup g_Popups[MAX_DAMAGE_POPUPS];

void PopupSystem_Init(void) {
    for (int i = 0; i < MAX_DAMAGE_POPUPS; i++) {
        g_Popups[i].active = false;
    }
}

void PopupSystem_Add(Vector2 position, int amount) {
    for (int i = 0; i < MAX_DAMAGE_POPUPS; i++) {
        if (!g_Popups[i].active) {
            g_Popups[i].position = position;
            g_Popups[i].amount = amount;
            g_Popups[i].timer = 0.0f;
            g_Popups[i].active = true;
            return;
        }
    }
}

void PopupSystem_Update(float deltaTime) {
    for (int i = 0; i < MAX_DAMAGE_POPUPS; i++) {
        if (g_Popups[i].active) {
            g_Popups[i].timer += deltaTime;
            g_Popups[i].position.y -= 40.0f * deltaTime; // Constant drift up

            if (g_Popups[i].timer >= 0.75f) {
                g_Popups[i].active = false;
            }
        }
    }
}

void PopupSystem_Draw(void) {
    Font defaultFont = GetFontDefault();
    
    for (int i = 0; i < MAX_DAMAGE_POPUPS; i++) {
        if (!g_Popups[i].active) continue;

        float t = g_Popups[i].timer;
        float scale = 1.0f;
        unsigned char alpha = 255;

        if (t < 0.2f) {
            // Scale up Phase: 1.0 -> 1.2
            float norm = t / 0.2f;
            scale = 1.0f + (norm * 0.2f);
        } else {
            // Scale down/Fade Phase: 1.2 -> 0.3
            float norm = (t - 0.2f) / 0.55f; // Remaining 0.55s
            scale = 1.2f - (norm * 0.9f);
            alpha = (unsigned char)(255 * (1.0f - norm));
        }

        char text[16];
        sprintf(text, "%d", g_Popups[i].amount);
        
        float baseSize = 24.0f;
        float finalSize = baseSize * scale;
        Vector2 textSize = MeasureTextEx(defaultFont, text, finalSize, 1.0f);
        Vector2 drawPos = { g_Popups[i].position.x - textSize.x/2, g_Popups[i].position.y - textSize.y/2 };

        Color outlineCol = (Color){ 0, 0, 0, alpha };
        Color textCol = (Color){ 255, 255, 0, alpha }; // Yellow

        // Draw shadow/border
        float offset = 2.0f * scale;
        DrawTextEx(defaultFont, text, (Vector2){ drawPos.x - offset, drawPos.y }, finalSize, 1.0f, outlineCol);
        DrawTextEx(defaultFont, text, (Vector2){ drawPos.x + offset, drawPos.y }, finalSize, 1.0f, outlineCol);
        DrawTextEx(defaultFont, text, (Vector2){ drawPos.x, drawPos.y - offset }, finalSize, 1.0f, outlineCol);
        DrawTextEx(defaultFont, text, (Vector2){ drawPos.x, drawPos.y + offset }, finalSize, 1.0f, outlineCol);

        // Draw main text
        DrawTextEx(defaultFont, text, drawPos, finalSize, 1.0f, textCol);
    }
}
