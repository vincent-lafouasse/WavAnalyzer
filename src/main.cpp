#include <algorithm>
#include <array>
#include <cassert>
#include <complex>
#include <iostream>

#include "colors/ColorMap.hpp"
#include "colors/Rgb.hpp"

#include <raylib.h>

typedef float Float;
typedef std::complex<Float> Complex;

namespace Constants {
[[maybe_unused]] static constexpr float fPi = M_PI;
[[maybe_unused]] static constexpr Complex cPi = fPi;
[[maybe_unused]] static constexpr float fTau = 2.0f * fPi;
[[maybe_unused]] static constexpr Complex cTau = fTau;
[[maybe_unused]] static constexpr Complex j(0.0, 1.0);
}  // namespace Constants

Complex nthRootOfUnity(usize n) {
    assert(n > 0);

    namespace C = Constants;

    return std::exp(C::cTau * C::j / static_cast<Complex>(n));
}

Complex cPow(Complex z, usize n) {
    if (n == 0) {
        return 1.0f;
    } else if (n == 1) {
        return z;
    }

    const Complex halfTerm = cPow(z, n / 2);

    if (n % 2 == 0) {
        return halfTerm * halfTerm;
    } else {
        return z * halfTerm * halfTerm;
    }
}

namespace OutOfPlace {
std::vector<Complex> fftRecurse(const Complex* input, usize size, usize step) {
    if (size == 1) {
        return {input[0]};
    }

    const std::vector<Complex> evens = fftRecurse(input, size / 2, 2 * step);
    const std::vector<Complex> odds =
        fftRecurse(input + step, size / 2, 2 * step);

    const Complex omega = nthRootOfUnity(size);

    std::vector<Complex> out(size, 0);
    for (usize i = 0; i < size / 2; ++i) {
        const Complex e_i = evens[i];
        const Complex o_i = cPow(omega, i) * odds[i];

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
