#include "Note.h"
#include "Track/Track.h"
#include "log.hpp"

#include <complex>
#include <iostream>
#include <numeric>

typedef std::complex<float> Complex;

const char* wav_path = "./wav/simple_chord.wav";

[[maybe_unused]] const float pi = std::acos(-1);
[[maybe_unused]] const Complex imaginary_unit(0.0, 1.0);
[[maybe_unused]] const Complex two_i_pi(0.0, 2 * pi);

std::vector<Complex> FFT_out_of_place(const std::vector<Complex>& input);

std::vector<Complex> FFT(const std::vector<float>& input)
{
    std::vector<Complex> complex_input{};
    for (float sample : input)
        complex_input.push_back(sample);

    return FFT_out_of_place(complex_input);
}

// a nasty FFT with a bunch of allocation in recursion
std::vector<Complex> FFT_out_of_place(const std::vector<Complex>& input)
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
    std::vector<float> signal = track.left;

    std::vector<Complex> dft = FFT(signal);
    std::cout << "FFT Done\n";

    std::vector<float> dft_amplitudes;
    for (const Complex& coefficient : dft)
        dft_amplitudes.push_back(std::norm(coefficient));

    float mean_amplitude =
        std::accumulate(dft_amplitudes.cbegin(), dft_amplitudes.cend(), 0.0);
    mean_amplitude /= dft_amplitudes.size();

    std::vector<size_t> dft_bucket_from_note{};
    size_t note = 0;
    while (Note(note).frequency() < 20000.0f)
    {
        dft_bucket_from_note.push_back(
            Note(note).corresponding_frequency_bucket(
                dft.size(), track.metadata.sample_rate));
        note++;
    }

    for (size_t note = dft_bucket_from_note.size(); note > 0; note--)
    {
        size_t bucket = dft_bucket_from_note.at(note - 1);
        size_t gain_from_mean = std::round(
            20.0 * std::log10(dft_amplitudes.at(bucket) / mean_amplitude));

        if (gain_from_mean > 0)
        {
            std::cout << Note(note).name() << "\t";
            for (size_t i = 0; i < gain_from_mean; i++)
                std::cout << "█";
            std::cout << '\n';
        }
    }

    return EXIT_SUCCESS;
}
