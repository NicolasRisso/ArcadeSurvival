#include "graphics/resource_manager.h"
#include <stdio.h>
#include <string.h>

#define MAX_TEXTURES 16

typedef struct {
    char key[32];
    Texture2D texture;
} TextureResource;

static TextureResource g_Textures[MAX_TEXTURES];
static int g_TextureCount = 0;

void Resources_Init(void) {
    g_TextureCount = 0;
    
    const char* keys[] = { TEX_PLAYER, TEX_ENEMIES, TEX_PICKUPS, TEX_MAP };
    const char* paths[] = { 
        "assets/sheets/PlayerSheet.png", 
        "assets/sheets/BatTexture.png", 
        "assets/sheets/pickups.png", 
        "assets/sheets/GrassTexture.png" 
    };
    
    for (int i = 0; i < 4; i++) {
        const char* finalPath = paths[i];
        
        // Path Fallback: Check local assets/ first, then ../assets/ if not found
        if (!FileExists(finalPath)) {
            static char fallbackBuffer[64];
            snprintf(fallbackBuffer, sizeof(fallbackBuffer), "../%s", paths[i]);
            if (FileExists(fallbackBuffer)) {
                finalPath = fallbackBuffer;
            }
        }

        if (FileExists(finalPath)) {
            Texture2D tex = LoadTexture(finalPath);
            if (tex.id > 0) {
                strncpy(g_Textures[g_TextureCount].key, keys[i], 32);
                g_Textures[g_TextureCount].texture = tex;
                g_TextureCount++;
                printf("[RESOURCES] Successfully loaded '%s' from: %s\n", keys[i], finalPath);
            } else {
                printf("[RESOURCES] ERROR: Raylib failed to load texture '%s' at: %s\n", keys[i], finalPath);
            }
        } else {
            printf("[RESOURCES] WARNING: Asset not found for '%s'. Expected at '%s' or '../%s'\n", 
                   keys[i], paths[i], paths[i]);
        }
    }
}

void Resources_Unload(void) {
    for (int i = 0; i < g_TextureCount; i++) {
        UnloadTexture(g_Textures[i].texture);
    }
    g_TextureCount = 0;
}

Texture2D Resources_GetTexture(const char* key) {
    for (int i = 0; i < g_TextureCount; i++) {
        if (strcmp(g_Textures[i].key, key) == 0) {
            return g_Textures[i].texture;
        }
    }
    return (Texture2D){ 0 }; // Return empty texture if not found
}
