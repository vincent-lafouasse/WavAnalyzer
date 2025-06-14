#include <algorithm>
#include <array>
#include <complex>
#include <iostream>
#include <numeric>

#include "colors/ColorMap.hpp"
#include "colors/Rgb.hpp"

#include <raylib.h>

typedef std::complex<float> Complex;

namespace Constants {
[[maybe_unused]] const float pi = std::acos(-1);
[[maybe_unused]] const Complex j(0.0, 1.0);
}  // namespace Constants

void fftRecurse(const Complex* in, Complex* out, usize size, usize step) {}

void fft(const Complex* in, Complex* out, usize size) {
    return fftRecurse(in, out, size, 1);
}

static constexpr int screenWidth = 1600;
static constexpr int screenHeight = 900;

static constexpr usize bufferSize = 1024;

std::array<float, bufferSize> globalBuffer{};
std::atomic<bool> bufferIsReady = false;

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

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);

        auto floatToHeight = [&](float f) {
            f = std::clamp(f, -1.0f, 1.0f);
            f = 0.5f * (f + 1.0f);
            return static_cast<int>(f * screenHeight);
        };

        if (bufferIsReady.load()) {
            localBuffer = globalBuffer;
            bufferIsReady.store(false);
        }

        BeginDrawing();
        ClearBackground(catpuccin::DarkGray.opaque());
        for (usize i = 0; i < bufferSize; ++i) {
            DrawPixel(i, floatToHeight(localBuffer[i]),
                      catpuccin::Pink.opaque());
        }
        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return EXIT_SUCCESS;
}
