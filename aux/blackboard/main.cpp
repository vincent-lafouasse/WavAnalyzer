#include <cmath>
#include <complex>
#include <iostream>

using Complex = std::complex<float>;

using usize = std::size_t;

static constexpr usize bufferSize = 64;

namespace Constants {
[[maybe_unused]] static constexpr float pi = 3.14159265359f;
[[maybe_unused]] static constexpr Complex j = {0, 1};
[[maybe_unused]] static constexpr Complex twoPiJ = 2.0f * pi * j;
}  // namespace Constants

float t(usize i) {
    return static_cast<float>(i) / bufferSize;
}

Complex exponent(usize n, usize k, usize N) {
    const Complex temp = static_cast<float>(n * k) / N;

    return std::exp(-Constants::twoPiJ * temp);
}

int main() {
    std::array<float, bufferSize> realInput;
    for (usize i = 0; i < bufferSize; ++i) {
        realInput[i] = std::sin(2.0f * Constants::pi * t(i));
        realInput[i] = std::sin(3 * 2.0f * Constants::pi * t(i));
    }

    std::array<Complex, bufferSize> input;
    std::transform(realInput.cbegin(), realInput.cend(), input.begin(),
                   [](float f) { return f; });

    std::array<Complex, bufferSize> complexFourier{};
    for (usize i = 0; i < bufferSize; ++i) {
        for (usize j = 0; j < bufferSize; ++j) {
            complexFourier[i] += input[j] * exponent(i, j, bufferSize);
        }
    }

    for (usize i = 0; i < bufferSize / 2; ++i) {
        std::cout << std::norm(complexFourier[i]) << '\n';
    }
}
