#include "Note.h"
#include "Track/Track.h"
#include "log.hpp"

#include "fftw3.h"

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

/*
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
        factor = std::exp(-Constants::two_i_pi * static_cast<float>(k) /
                          static_cast<float>(N));

        output[k] = evens[k] + factor * odds[k];
        output[k + N / 2] = evens[k] + factor * odds[k];
    }

    return output;
}

float window_function(size_t index, size_t window_size)
{
    (void)index;
    (void)window_size;
    return 1.0;
}

std::vector<float> FFT_slice(const std::vector<float>& input,
                             size_t offset,
                             size_t window_size)
{
    assert(offset + window_size <= input.size());

    std::vector<float> windowed_input(window_size);
    for (size_t i = 0; i < window_size; i++)
    {
        windowed_input[i] = input.at(offset + i);
        windowed_input[i] *= window_function(i, window_size);
    }

    std::vector<Complex> dft = FFT(windowed_input);

    std::vector<float> real_dft{};
    for (Complex coeff : dft)
        real_dft.push_back(coeff.real());

    return real_dft;
}

template <typename T>
inline float mean(const std::vector<T>& v)
{
    return std::accumulate(v.cbegin(), v.cend(), 0.0) /
           static_cast<float>(v.size());
}

size_t frequency_to_bucket(float frequency, size_t dft_size, float sample_rate)
{
    return frequency * sample_rate / dft_size;
}

float gain_amplitude(float amplitude, float reference)
{
    return std::round(20.0 * std::log10(amplitude / reference));
}

template <typename T>
T percentile(std::vector<T> vec, float percent)
{
    std::sort(vec.begin(), vec.end());
    size_t index = vec.size() * percent / 100.0;
    return vec.at(index);
}

template <typename T>
void print_n(T object, size_t n)
{
    for (size_t i = 0; i < n; i++)
        std::cout << object;
}
*/

size_t crop_to_pow2(size_t sz)
{
    size_t power = 0;

    while ((1 << power) < sz)
        power++;

    power -= 1;
    return (1 << power);
}

struct SpectrumAnalyzer
{
    SpectrumAnalyzer(const std::vector<float>& signal, float sample_rate)
    {
        input_size = crop_to_pow2(signal.size());
        input =
            std::vector<double>(signal.cbegin(), signal.cbegin() + input_size);
        output = std::vector<double>(input_size);

        input_sample_rate = sample_rate;
        const double frequency_unit = input_sample_rate / input_size;

        first_bin = std::ceil(20.0 / frequency_unit);

        const size_t last_bin = std::min(
            input_size, static_cast<size_t>(std::ceil(20000 / frequency_unit)));
        n_bins = last_bin - first_bin;
    }

    ~SpectrumAnalyzer(){};

    void execute_fftw3_NOT_MINE()
    {
        const unsigned int plan_flags = FFTW_ESTIMATE;
        const fftw_r2r_kind kind = FFTW_HC2R;
        fftw_plan plan = fftw_plan_r2r_1d(input_size, &(input[0]), &(output[0]),
                                          kind, plan_flags);
        fftw_execute(plan);
        fftw_destroy_plan(plan);
    }
    void write()
    {
        std::ofstream csv;
        csv.open("fft.csv");
        const double frequency_unit = input_sample_rate / input_size;
        for (size_t i = 0; i < input_size / 2; i++)
        {
            csv << frequency_unit * i << ",";
        }
        csv << std::endl;
        for (size_t i = 0; i < input_size / 2; i++)
        {
            csv << output[i] << ",";
        }
        csv << std::endl;
        csv.close();
        std::cout << "fft csv written\n";
    }

    std::vector<double> input;
    std::vector<double> output;
    size_t input_size;
    float input_sample_rate;
    size_t first_bin;  // frequency offset, ignore below 20 Hz
    size_t n_bins;     // nnumber of frequency bins to fetch
};

int main()
{
    std::cout << "Loading " << wav_path << "\n";
    Track track = Track::from_wav(wav_path);

    SpectrumAnalyzer spectrum(track.left, track.metadata.sample_rate);
    spectrum.execute_fftw3_NOT_MINE();
    spectrum.write();

    /*
    std::vector<float> dft_real = FFT_slice(signal, 0, N);
    std::cout << "FFT Done\n";
    write_to_csv(dft_real, "real_dft.csv");

    float mean_amplitude = mean(dft_real);

    std::vector<size_t> bucket_indices{};
    size_t note = 0;
    while (Note(note).frequency() < 20000.0f)
    {
        bucket_indices.push_back(
            frequency_to_bucket(Note(note).frequency(), dft_real.size(),
                                track.metadata.sample_rate));
        note++;
    }

    for (size_t note = bucket_indices.size(); note > 0; note--)
    {
        size_t bucket = bucket_indices.at(note - 1);
        size_t gain_from_mean =
            gain_amplitude(dft_real.at(bucket), mean_amplitude);

        if (gain_from_mean > 0)
        {
            std::cout << Note(note).name() << "\t";
            print_n("█", gain_from_mean);
            std::cout << '\n';
        }
    }
    */

    return EXIT_SUCCESS;
}
