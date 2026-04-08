#include "graphics/resource_manager.h"
#include <stdio.h>
#include <string.h>

#define MAX_TEXTURES 16

typedef struct {
    char key[32];
    Texture2D texture;
} TextureResource;

typedef struct {
    char key[32];
    Sound sound;
} SoundResource;

typedef struct {
    char key[32];
    Music music;
} MusicResource;

static TextureResource g_Textures[MAX_TEXTURES];
static int g_TextureCount = 0;

static SoundResource g_Sounds[MAX_TEXTURES];
static int g_SoundCount = 0;

static MusicResource g_Music[MAX_TEXTURES];
static int g_MusicCount = 0;

static void ProcessPath(const char* baseName, char* outPath, size_t outSize) {
    if (FileExists(baseName)) {
        strncpy(outPath, baseName, outSize);
    } else {
        snprintf(outPath, outSize, "../%s", baseName);
    }
}

void Resources_Init(void) {
    g_TextureCount = 0;
    g_SoundCount = 0;
    g_MusicCount = 0;
    
    // 1. Textures
    const char* texKeys[] = { TEX_PLAYER, TEX_ENEMIES, TEX_PICKUPS, TEX_MAP };
    const char* texPaths[] = { 
        "assets/sheets/PlayerSheet.png", 
        "assets/sheets/BatTexture.png", 
        "assets/sheets/pickups.png", 
        "assets/sheets/GrassTexture.png" 
    };
    
    for (int i = 0; i < 4; i++) {
        char finalPath[128];
        ProcessPath(texPaths[i], finalPath, sizeof(finalPath));

        if (FileExists(finalPath)) {
            Texture2D tex = LoadTexture(finalPath);
            if (tex.id > 0) {
                strncpy(g_Textures[g_TextureCount].key, texKeys[i], 32);
                g_Textures[g_TextureCount].texture = tex;
                g_TextureCount++;
                printf("[RESOURCES] Loaded Texture '%s'\n", texKeys[i]);
            }
        }
    }

    // 2. Sounds
    const char* sndKeys[] = { SND_EXPLOSION, SND_XP_GAIN, SND_LEVEL_UP, SND_ENEMY_HIT, SND_PLAYER_HIT };
    const char* sndPaths[] = { 
        "assets/sounds/ExplosionAudio.ogg", 
        "assets/sounds/XpGainAudio.ogg", 
        "assets/sounds/LevelUpAudio.ogg",
        "assets/sounds/DamageAudio.mp3",
        "assets/sounds/PlayerDamageAudio.ogg"
    };

    for (int i = 0; i < 5; i++) {
        char finalPath[128];
        ProcessPath(sndPaths[i], finalPath, sizeof(finalPath));

        if (FileExists(finalPath)) {
            Sound snd = LoadSound(finalPath);
            if (snd.frameCount > 0) {
                strncpy(g_Sounds[g_SoundCount].key, sndKeys[i], 32);
                g_Sounds[g_SoundCount].sound = snd;
                g_SoundCount++;
                printf("[RESOURCES] Loaded Sound '%s'\n", sndKeys[i]);
            }
        }
    }

    // 3. Music
    const char* musKeys[] = { MUS_COMBAT };
    const char* musPaths[] = { "assets/sounds/CombatMusic.ogg" };

    for (int i = 0; i < 1; i++) {
        char finalPath[128];
        ProcessPath(musPaths[i], finalPath, sizeof(finalPath));

        if (FileExists(finalPath)) {
            Music mus = LoadMusicStream(finalPath);
            if (mus.frameCount > 0) {
                strncpy(g_Music[g_MusicCount].key, musKeys[i], 32);
                g_Music[g_MusicCount].music = mus;
                g_MusicCount++;
                printf("[RESOURCES] Loaded Music '%s'\n", musKeys[i]);
            }
        }
    }
}

void Resources_Unload(void) {
    for (int i = 0; i < g_TextureCount; i++) UnloadTexture(g_Textures[i].texture);
    for (int i = 0; i < g_SoundCount; i++) UnloadSound(g_Sounds[i].sound);
    for (int i = 0; i < g_MusicCount; i++) UnloadMusicStream(g_Music[i].music);
    
    g_TextureCount = 0;
    g_SoundCount = 0;
    g_MusicCount = 0;
}

Texture2D Resources_GetTexture(const char* key) {
    for (int i = 0; i < g_TextureCount; i++) {
        if (strcmp(g_Textures[i].key, key) == 0) return g_Textures[i].texture;
    }
    return (Texture2D){ 0 };
}

Sound Resources_GetSound(const char* key) {
    for (int i = 0; i < g_SoundCount; i++) {
        if (strcmp(g_Sounds[i].key, key) == 0) return g_Sounds[i].sound;
    }
    return (Sound){ 0 };
}

Music Resources_GetMusic(const char* key) {
    for (int i = 0; i < g_MusicCount; i++) {
        if (strcmp(g_Music[i].key, key) == 0) return g_Music[i].music;
    }
    return (Music){ 0 };
}
