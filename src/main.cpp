#include <array>
#include <iostream>
#include <iterator>

#include "colors/ColorMap.hpp"
#include "fft.h"

#include <raylib.h>

static constexpr int screenWidth = 1600;
static constexpr int screenHeight = 900;

static constexpr usize bufferSize = 4096;

std::array<float, bufferSize> globalBuffer{};
std::atomic<bool> bufferIsReady = false;

template <class It>
It findMax(It start, It end) {
    if (start == end) {
        return end;
    }

    auto maxValue = *start++;
    It maxPos = start;

    while (start != end) {
        if (maxValue < *start) {
            maxValue = *start;
            maxPos = start;
        }
    }

    return maxPos;
}

void callback(void* buffer, u32 frames) {
    const float* samples = static_cast<const float*>(buffer);

    static usize bufferIndex = 0;

    if (!bufferIsReady.load()) {
        for (usize i = 0; i < frames; ++i) {
            const float sample = samples[2 * i];
            globalBuffer[bufferIndex] = sample;
            ++bufferIndex;

            if (bufferIndex == bufferSize) {
                bufferIsReady.store(true);
                bufferIndex = 0;
                break;
            }
        }
    }
}

int main(int ac, char** av) {
    if (ac != 2) {
        std::cout << "Usage: " << av[0] << " WAV_PATH\n";
        std::exit(0);
    }

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Multisample Anti-Aliasing

    InitWindow(screenWidth, screenHeight, "spectrogram");
    SetTargetFPS(60);

    InitAudioDevice();
    AttachAudioMixedProcessor(callback);

    Music music = LoadMusicStream(av[1]);
    music.looping = false;

    PlayMusicStream(music);

    ColorMap cmap = ColorMap::Viridis();

    std::array<float, bufferSize> localBuffer{};
    std::vector<float> fft(bufferSize, 0);

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);

        if (bufferIsReady.load()) {
            localBuffer = globalBuffer;
            bufferIsReady.store(false);
            fft = OutOfPlace::fft(&localBuffer[0], bufferSize);
        }

        auto floatToHeight = [&](float f) {
            return static_cast<int>(f * screenHeight);
        };

        BeginDrawing();
        ClearBackground(catpuccin::DarkGray.opaque());
        for (usize i = 0; i < bufferSize / 2; ++i) {
            static constexpr int barWidth = 4;
            const Color c =
                cmap.get(2.0f * static_cast<float>(i) / bufferSize).opaque();
            const int h = floatToHeight(fft[i]);

            DrawRectangle(i * barWidth, screenHeight - h, barWidth, h, c);
        }
        DrawFPS(0, 0);
        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return EXIT_SUCCESS;
}
