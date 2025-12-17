#pragma once

#include "types.h"

namespace fft {
std::vector<Complex> forward(const std::vector<Complex>& input);

std::vector<float> r2c(const std::vector<float>& input);
}
