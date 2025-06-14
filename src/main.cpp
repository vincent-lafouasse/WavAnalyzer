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

namespace OutOfPlace {
std::vector<Complex> fftRecurse(const Complex* input, usize size, usize step) {
    if (size == 1) {
        return {input[0]};
    }

    const std::vector<Complex> evens = fftRecurse(input, size / 2, 2 * step);
    const std::vector<Complex> odds =
        fftRecurse(input + step, size / 2, 2 * step);

    std::vector<Complex> out(size, 0);
    for (usize i = 0; i < size / 2; ++i) {
        const Complex e_i = input[i];
        const Complex o_i =
            input[i + size / 2] *
            std::exp(-Constants::j *
                     static_cast<Complex>(2.0 * Constants::pi * i / size));

        out[i] = e_i + o_i;
        out[i + size / 2] = e_i - o_i;
    }

    return out;
}

std::vector<float> fft(const float* input, usize size) {
    std::vector<Complex> complexInput(size, 0);
    std::transform(input, input + size, complexInput.begin(),
                   [](float e) { return e; });

    std::vector<Complex> complexFourier = fftRecurse(&complexInput[0], size, 1);

    std::vector<float> out(size, 0);
    std::transform(complexFourier.cbegin(), complexFourier.cend(), out.begin(),
                   [](Complex z) { return std::norm(z); });

    return out;
}

}  // namespace OutOfPlace

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
    std::vector<float> fft(0, bufferSize);

    bool didTheThing = false;

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);

        if (bufferIsReady.load() && !didTheThing) {
            localBuffer = globalBuffer;
            bufferIsReady.store(false);
            fft = OutOfPlace::fft(&localBuffer[0], bufferSize);
            for (float f : fft) {
                std::cout << f << ",";
            }
            std::cout << '\n';
            didTheThing = true;
        }

        auto floatToHeight = [&](float f) {
            return static_cast<int>((1.0f - f) * screenHeight);
        };

        BeginDrawing();
        ClearBackground(catpuccin::DarkGray.opaque());
        if (didTheThing) {
            for (usize i = 0; i < bufferSize; ++i) {
                DrawPixel(i, floatToHeight(fft[i]), catpuccin::Pink.opaque());
            }
        }
        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return EXIT_SUCCESS;
}
