#pragma once
#include "libs/miniaudio.h"
#include <string>

void initAudio();
void playWav(const std::string& path);
void shutdownAudio();
