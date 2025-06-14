#include <cmath>
#include <complex>
#include <iostream>

namespace Constants {
[[maybe_unused]] static constexpr float pi = 3.14159265359f;
[[maybe_unused]] static constexpr Complex j = {0, 1};
[[maybe_unused]] static constexpr Complex twoPiJ = 2.0f * pi * j;
}  // namespace Constants

using Complex = std::complex<float>;

using usize = std::size_t;

static constexpr usize bufferSize = 8;

float t(usize i) {
    return static_cast<float>(i) / bufferSize;
}

int main() {
    std::array<float, bufferSize> realInput;
    for (usize i = 0; i < bufferSize; ++i) {
        realInput[i] = std::sin(2.0f * Constants::pi * t(i));
    }

    std::array<Complex, bufferSize> input;
    std::transform(realInput.cbegin(), realInput.cend(), input.begin(),
                   [](float f) { return f; });

    for (Complex x : input) {
        std::cout << x << '\n';
    }
}
