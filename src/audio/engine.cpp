#define MINIAUDIO_IMPLEMENTATION
#include "libs/miniaudio.h"
#include "audio/engine.h"

#include <iostream>

static ma_engine g_engine;
static bool g_audio_init = false;

void initAudio() {
    if (!g_audio_init) {
        if (ma_engine_init(NULL, &g_engine) == MA_SUCCESS) {
            g_audio_init = true;
            std::cout << "[Audio] Engine initialized.\n";
        }
        else {
            std::cerr << "[AudioError] Failed to initialize engine.\n";
        }
    }
}

void playWav(const std::string& path) {
    if (!g_audio_init) initAudio();

    if (ma_engine_play_sound(&g_engine, path.c_str(), NULL) != MA_SUCCESS) {
        std::cerr << "[AudioError] Failed to play: " << path << "\n";
        return;
    }
    std::cout << "[Audio] Playing " << path << "\n";
}

void shutdownAudio() {
    if (g_audio_init) {
        ma_engine_uninit(&g_engine);
        g_audio_init = false;
    }
}
