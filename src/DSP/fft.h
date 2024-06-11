#pragma once

#include <complex>
#include <vector>

typedef std::complex<float> Complex;

std::vector<Complex> ugly_fft(const std::vector<float>& input);
