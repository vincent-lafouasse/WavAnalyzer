#include "DSP/Note.h"
#include "DSP/SpectrumAnalyzer.h"
#include "Track/Track.h"
#include "log.hpp"

#include <complex>
#include <iostream>
#include <numeric>

typedef std::complex<float> Complex;

const char* wav_path = "./wav/long_a440_sine.wav";
namespace Constants
{
[[maybe_unused]] const float pi = std::acos(-1);
[[maybe_unused]] const Complex imaginary_unit(0.0, 1.0);
[[maybe_unused]] const Complex two_i_pi(0.0, 2 * pi);
}  // namespace Constants

int main()
{
    std::cout << "Loading " << wav_path << "\n";
    Track track = Track::from_wav(wav_path);

    SpectrumAnalyzer spectrum(track.left, track.metadata.sample_rate);
    spectrum.execute_fftw3_NOT_MINE();
    spectrum.write("fftw3_fft.csv");

    return EXIT_SUCCESS;
}
