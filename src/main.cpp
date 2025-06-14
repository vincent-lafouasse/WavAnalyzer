#include <complex>
#include <iostream>
#include "colors/Rgb.hpp"

#include <raylib.h>

typedef std::complex<float> Complex;

namespace Constants
{
[[maybe_unused]] const float pi = std::acos(-1);
[[maybe_unused]] const Complex imaginary_unit(0.0, 1.0);
[[maybe_unused]] const Complex two_i_pi(0.0, 2 * pi);
}  // namespace Constants

static constexpr int screenWidth = 1600;
static constexpr int screenHeight = 900;

int main(int ac, char** av)
{
    if (ac != 2)
    {
        std::cout << "Usage: " << av[0] << " WAV_PATH\n";
        std::exit(0);
    }

    InitWindow(screenWidth, screenHeight, "spectrogram");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(catpuccin::DarkGray.opaque());
        EndDrawing();
    }

    CloseWindow();

    return EXIT_SUCCESS;
}
