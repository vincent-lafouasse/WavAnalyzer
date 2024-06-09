#include "Track/Track.h"

#include <complex>
#include <fstream>
#include <iostream>

typedef std::complex<float> Complex;

const char* wav_path = "./wav/nice_chord.wav";

[[maybe_unused]] const float pi = std::acos(-1);
[[maybe_unused]] const Complex imaginary_unit(0.0, 1.0);
[[maybe_unused]] const Complex two_i_pi(0.0, 2 * pi);

struct SpectrogramParameters
{
    u32 frequency_resolution() const;
    u32 window_duration_ms() const;

    u32 sample_rate;
    u32 window_size;
    u32 time_increment;
};

template<typename T>
void write_to_csv(const std::vector<T>& data);

template <typename T>
void log(T object, const char* name)
{
    std::cout << name << " : " << object << '\n';
    std::cout.flush();
}

// a nasty FFT with a bunch of allocation in recursion
std::vector<Complex> FFT_out_of_place(std::vector<Complex> input)
{
    const size_t N = input.size();
    if (N <= 1)
        return input;

    std::vector<Complex> evens;
    std::vector<Complex> odds;
    for (size_t i = 0; i < N; i++)
    {
        if (i % 2 == 0)
            evens.push_back(input[i]);
        else
            odds.push_back(input[i]);
    }

    evens = FFT_out_of_place(evens);
    odds = FFT_out_of_place(odds);

    std::vector<Complex> output(N);

    Complex factor;
    for (size_t k = 0; k < N / 2; k++)
    {
        factor =
            std::exp(-two_i_pi * static_cast<float>(k) / static_cast<float>(N));

        output[k] = evens[k] + factor * odds[k];
        output[k + N / 2] = evens[k] + factor * odds[k];
    }

    return output;
}

int main()
{
    Track track = Track::from_wav(wav_path);
    std::vector<Complex> signal;
    for (float sample : track.left)
        signal.push_back(sample);

    float total_energy = 0;
    for (Complex sample: signal)
        total_energy += std::norm(sample);
    log(total_energy, "total energy");

    std::vector<Complex> dft = FFT_out_of_place(signal);
    std::cout << "FFT Done\n";

    std::vector<float> dft_amplitudes;
    for (const Complex& coefficient : dft)
        dft_amplitudes.push_back(std::norm(coefficient));

    write_to_csv(dft_amplitudes);

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


template<typename T>
void write_to_csv(const std::vector<T>& data)
{
    std::ofstream csv;
    csv.open("signal.csv");
    for (const T& cell: data)
    {
        csv << cell << ",";
    }
    csv << std::endl;
    csv.close();
}
