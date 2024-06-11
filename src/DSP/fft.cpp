#include "fft.h"

#include <cmath>

namespace PossDSP
{

namespace constants
{
constexpr Complex J = {0, 1};
const Complex two_pi = {2.0f * std::acosf(-1), 0};
}  // namespace constants

std::vector<Complex> ugly_fft(const std::vector<Complex>& input)
{
    if (input.size() <= 1)
        return input;

    const size_t N = input.size();

    std::vector<Complex> evens;
    std::vector<Complex> odds;
    for (size_t i = 0; i < input.size(); i++)
    {
        if (i % 2 == 0)
            evens.push_back(input[i]);
        else
            odds.push_back(input[i]);
    }

    evens = ugly_fft(evens);
    odds = ugly_fft(odds);

    const Complex w_N =
        std::exp(-constants::J * constants::two_pi / static_cast<Complex>(N));
    std::vector<Complex> out;
    for (size_t k = 0; k < N / 2; k++)
    {
        out[k] = evens[k] + odds[k] * std::pow(w_N, static_cast<Complex>(k));
        out[k + N / 2] =
            evens[k] - odds[k] * std::pow(w_N, static_cast<Complex>(k));
    }

    return out;
}
}  // namespace PossDSP
