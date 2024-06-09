#include "Track/Track.h"

#include <complex>
#include <iomanip>
#include <iostream>

typedef std::complex<float> Complex;

const char* wav_path = "./wav/nice_chord.wav";

struct SpectrogramParameters
{
    u32 frequency_resolution() const;
    u32 window_duration_ms() const;

    u32 sample_rate;
    u32 window_size;
    u32 time_increment;
};

template <typename T>
void log(T object, const char* name)
{
    std::cout << name << " : " << object << '\n';
    std::cout.flush();
}

std::vector<Complex> FFT_out_of_place(std::vector<Complex> input)
{
    if (input.size() <= 1)
        return input;

    std::vector<Complex> output;

    return output;
}

int main()
{
    Track track = Track::from_wav(wav_path);
    std::vector<Complex> signal;
    for (float sample: track.left)
        signal.push_back(sample);

    [[maybe_unused]] const size_t input_size = signal.size();

    [[maybe_unused]] const float pi = std::acos(-1);
    [[maybe_unused]] const Complex imaginary_unit(0.0, 1.0);
    [[maybe_unused]] const Complex two_i_pi(0.0, 2 * pi);
    [[maybe_unused]] const size_t N = (1 << 17);
    [[maybe_unused]] const float coverage =
        1.0 - (input_size - N) / static_cast<float>(N);
    log(N, "N");
    log(input_size - N, "ignored samples");
    log(100.0 * coverage, "coverage");

    /*
    std::vector<Complex> dft;
    for (size_t k = 0; k < N; k++)  // a naive n^2 DFT
    {
        Complex X_k = 0;

        for (size_t n = 0; n < N; n++)
        {
            X_k += signal[n] *
                   std::exp(-imaginary_unit * two_pi *
                            static_cast<float>(k * n) / static_cast<float>(N));
        }

        dft.push_back(X_k);
        float completion = 100.0 * k / static_cast<float>(N);
        std::cout << completion << '\n';
    }
    */

    return EXIT_SUCCESS;
}

u32 SpectrogramParameters::frequency_resolution() const
{
    return sample_rate / window_size;
}

u32 SpectrogramParameters::window_duration_ms() const
{
    return 1000 * window_size / static_cast<float>(sample_rate);
}
