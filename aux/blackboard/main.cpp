#include <cmath>
#include <complex>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

#include <algorithm>
#include <array>

using Complex = std::complex<float>;

using usize = std::size_t;

static constexpr usize bufferSize = 64;

namespace Constants {
[[maybe_unused]] static constexpr float pi = 3.14159265359f;
[[maybe_unused]] static constexpr Complex j = {0, 1};
[[maybe_unused]] static constexpr Complex twoPiJ = 2.0f * pi * j;
}  // namespace Constants

template <class It>
void writeToCsv(It start, It end, const char* filename) {
    std::ofstream outfile(filename);

    for (It it = start; it != end; ++it) {
        outfile << *it << ',';
    }
    outfile << '\n';
}

Complex exponent(usize n, usize k, usize N) {
    const Complex temp = static_cast<float>(n * k) / N;

    return std::exp(-Constants::twoPiJ * temp);
}

namespace Naive {
std::array<Complex, bufferSize> dft(
    const std::array<Complex, bufferSize> input) {
    std::array<Complex, bufferSize> complexFourier{};
    for (usize i = 0; i < bufferSize; ++i) {
        for (usize j = 0; j < bufferSize; ++j) {
            complexFourier[i] += input[j] * exponent(i, j, bufferSize);
        }
    }

    return complexFourier;
}
}  // namespace Naive

std::array<Complex, bufferSize> makeInput() {
    srand(static_cast<unsigned>(time(0)));
    static constexpr float noiseAmp = 0.0f;

    static constexpr float omega0 = 2.0f * Constants::pi / bufferSize;
    auto omega = [](auto k) { return static_cast<float>(omega0 * k); };

    std::array<float, bufferSize> realInput;
    for (usize i = 0; i < bufferSize; ++i) {
        realInput[i] = std::sin(omega(1) * i);
        realInput[i] += std::sin(omega(3) * i);
        const float noise =
            (-noiseAmp) +
            static_cast<float>(rand()) /
                (static_cast<float>(RAND_MAX / (2.0f * noiseAmp)));
        realInput[i] += noise;
    }

    writeToCsv(realInput.cbegin(), realInput.cend(), "input.csv");

    std::array<Complex, bufferSize> input;
    std::transform(realInput.cbegin(), realInput.cend(), input.begin(),
                   [](float f) { return f; });

    return input;
}

int main() {
    std::array<Complex, bufferSize> input = makeInput();

    std::array<Complex, bufferSize> complexDft = Naive::dft(input);

    static constexpr usize spectrumSize = (bufferSize / 2) - 1;
    std::array<float, spectrumSize> spectrum;
    std::transform(complexDft.cbegin() + 1, complexDft.cbegin() + bufferSize / 2,
                   spectrum.begin(),
                   [](const auto& e) { return std::norm(e); });

    for (usize i = 0; i < spectrum.size(); ++i) {
        std::clog << i + 1 << ":\t" << spectrum[i] << '\n';
    }

    writeToCsv(spectrum.cbegin(), spectrum.cend(), "dft.csv");
}
