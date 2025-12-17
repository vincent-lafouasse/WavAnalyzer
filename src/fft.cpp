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

namespace fft {
std::vector<Complex> forward(const std::vector<Complex>& input) {
    return fftRecurse(input.data(), input.size(), 1);
}
}
