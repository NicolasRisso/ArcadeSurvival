#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "raylib.h"

void AudioManager_Init(void);
void AudioManager_Update(void);
void AudioManager_PlaySfx(const char* key);
void AudioManager_PlaySfxThrottled(const char* key, int limitPerFrame);

#endif // AUDIO_MANAGER_H
