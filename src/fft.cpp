#include "fft.h"

#include <cassert>
#include <vector>

#include "constants.h"

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

    const usize sz = size / 2;
    std::vector<float> out(sz, 0);
    std::transform(complexFourier.cbegin(), complexFourier.cbegin() + sz,
                   out.begin(), [](Complex z) { return std::norm(z); });

    return out;
}

}  // namespace OutOfPlace
