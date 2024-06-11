#pragma once

#include <complex>
#include <vector>

typedef std::complex<float> Complex;

namespace PossDSP
{
std::vector<Complex> ugly_fft(const std::vector<Complex>& input);
}
