#include <algorithm>
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

std::atomic<float> rms = 0;

void callback(void* buffer, u32 frames) {
    const float* samples = static_cast<const float*>(buffer);

    float squares = std::reduce(samples, samples + 2 * frames, 0.0f,
                                [](float acc, float e) { return acc + e * e; });

    rms.store(std::sqrt(squares));
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

        const float x = std::clamp(rms.load(), 0.0f, 1.0f);
        const int barHeight = static_cast<int>(x * screenHeight);
        const int barWidth = 100;

        BeginDrawing();
        ClearBackground(catpuccin::DarkGray.opaque());
        DrawRectangle(0, screenHeight - barHeight, barWidth, barHeight,
                      cmap.get(x).opaque());
        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return EXIT_SUCCESS;
}
