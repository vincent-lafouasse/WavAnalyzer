#include "Track/Track.h"

#include <complex>
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
    std::cout << name << " :\t" << object << '\n';
    std::cout.flush();
}

int main()
{
    Track track = Track::from_wav(wav_path);
    std::vector<float> signal = track.left;

    const float two_pi = 2 * std::acos(-1);
    const Complex imaginary_unit(0.0, 1.0);
    const size_t N = signal.size();

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

    log(signal.size(), "input size");
    log(dft.size(), "dft size");

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
