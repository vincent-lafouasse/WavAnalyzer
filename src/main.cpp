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
[[maybe_unused]] const Complex imaginary_unit(0.0, 1.0);
[[maybe_unused]] const Complex two_i_pi(0.0, 2 * pi);
}  // namespace Constants

static constexpr int screenWidth = 1600;
static constexpr int screenHeight = 900;

static constexpr usize fftBufferSize = 1024;

std::array<float, fftBufferSize> fftBuffer{};
usize fftBufferIndex = 0;
std::atomic<bool> bufferIsReady = false;

void callback(void* buffer, u32 frames) {
    const float* samples = static_cast<const float*>(buffer);

    if (!bufferIsReady.load()) {
        for (usize i = 0; i < frames; ++i) {
            const float sample = samples[2 * i];
            fftBuffer[fftBufferIndex] = sample;
            ++fftBufferIndex;

            if (fftBufferIndex == fftBufferSize) {
                bufferIsReady.store(true);
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

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);

        auto floatToHeight = [&](float f) {
            f = std::clamp(f, -1.0f, 1.0f);
            f = 0.5f * (f + 1.0f);
            return static_cast<int>(f * screenHeight);
        };

        BeginDrawing();
        ClearBackground(catpuccin::DarkGray.opaque());
        if (bufferIsReady.load()) {
            for (usize i = 0; i < fftBufferSize; ++i) {
                DrawPixel(i, floatToHeight(fftBuffer[i]),
                          catpuccin::Pink.opaque());
            }
        }
        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return EXIT_SUCCESS;
}
