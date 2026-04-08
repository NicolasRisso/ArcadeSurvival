#include "game/audio/audio_manager.h"
#include "graphics/resource_manager.h"
#include "raylib.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    char key[32];
    int count;
    double lastTime;
} SfxThrottle;

#define MAX_THROTTLES 16
static SfxThrottle g_Throttles[MAX_THROTTLES];
static int g_ThrottleCount = 0;

static Music g_BackgroundMusic;
static bool g_MusicStarted = false;

void AudioManager_Init(void) {
    g_ThrottleCount = 0;
    memset(g_Throttles, 0, sizeof(g_Throttles));

    g_BackgroundMusic = Resources_GetMusic(MUS_COMBAT);
    if (g_BackgroundMusic.frameCount > 0) {
        SetMusicVolume(g_BackgroundMusic, 0.4f);
        PlayMusicStream(g_BackgroundMusic);
        g_MusicStarted = true;
    }
}

void AudioManager_Update(void) {
    if (g_MusicStarted) {
        UpdateMusicStream(g_BackgroundMusic);
    }
}

static void PlayInternal(const char* key) {
    Sound snd = Resources_GetSound(key);
    if (snd.frameCount > 0) {
        // Random Pitch Modulation (+/- 15%) to break repetitive feel
        float pitch = 0.85f + ((float)GetRandomValue(0, 30) / 100.0f);
        SetSoundPitch(snd, pitch);
        PlaySound(snd);
    }
}

void AudioManager_PlaySfx(const char* key) {
    PlayInternal(key);
}

void AudioManager_PlaySfxThrottled(const char* key, int limitPerWindow) {
    double currentTime = GetTime();
    
    // Find or create throttle entry
    int idx = -1;
    for (int i = 0; i < g_ThrottleCount; i++) {
        if (strcmp(g_Throttles[i].key, key) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        if (g_ThrottleCount < MAX_THROTTLES) {
            idx = g_ThrottleCount++;
            strncpy(g_Throttles[idx].key, key, 32);
            g_Throttles[idx].lastTime = -1.0;
        } else {
            // Fallback to unthrottled if we run out of slots (unlikely)
            PlayInternal(key);
            return;
        }
    }

    // Reset count every 0.1 seconds
    if (currentTime - g_Throttles[idx].lastTime > 0.1) {
        g_Throttles[idx].lastTime = currentTime;
        g_Throttles[idx].count = 0;
    }

    if (g_Throttles[idx].count < limitPerWindow) {
        g_Throttles[idx].count++;
        PlayInternal(key);
    }
}
