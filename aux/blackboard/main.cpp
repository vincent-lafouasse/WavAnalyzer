#include <cmath>
#include <complex>
#include <iostream>

using Complex = std::complex<float>;

using usize = std::size_t;

static constexpr usize bufferSize = 8;
static constexpr float pi = 3.14159265359f;

float t(usize i) {
    return static_cast<float>(i) / bufferSize;
}

int main() {
    std::array<float, bufferSize> input;
    for (usize i = 0; i < bufferSize; ++i) {
        input[i] = std::sin(2.0f * pi * t(i));
    }

    for (float x : input) {
        std::cout << x << ',';
    }
    std::cout << '\n';
}
