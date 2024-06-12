#include "DSP/SpectrumAnalyzer.h"
#include "Track/Track.h"

#include <complex>
#include <iostream>

typedef std::complex<float> Complex;

namespace Constants
{
[[maybe_unused]] const float pi = std::acos(-1);
[[maybe_unused]] const Complex imaginary_unit(0.0, 1.0);
[[maybe_unused]] const Complex two_i_pi(0.0, 2 * pi);
}  // namespace Constants

int main(int ac, char** av)
{
    if (ac != 2)
    {
        std::cout << "Usage: " << av[0] << " WAV_PATH\n";
        std::exit(0);
    }

    const char* wav_path = av[1];
    std::cout << "Loading " << wav_path << "\n";
    Track track = Track::from_wav(wav_path);

    SpectrumAnalyzer spectrum(track.left, track.metadata.sample_rate);
    spectrum.execute_fft();
    spectrum.write_sampled("my_fft_sampled.csv");

    return EXIT_SUCCESS;
}
