#include "SpectrumAnalyzer.h"
#include "fft.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>

static size_t crop_to_pow2(size_t sz);

static float amplitude_at(float frequency,
                          const std::vector<double>& amplitudes,
                          float sample_rate)
{
    float frequency_unit = sample_rate / amplitudes.size();
    float index_f = frequency / frequency_unit;

    // lerp
    float low = amplitudes[std::floor(index_f)];
    float high = amplitudes[std::ceil(index_f)];
    float offset = index_f - std::floorf(index_f);

    return low + offset * (high - low);
}

SpectrumAnalyzer::SpectrumAnalyzer(const std::vector<float>& signal,
                                   float sample_rate)
{
    input_size = crop_to_pow2(signal.size());
    input = std::vector<double>(signal.cbegin(), signal.cbegin() + input_size);
    output = std::vector<double>(input_size);

    input_sample_rate = sample_rate;
    frequency_unit = input_sample_rate / input_size;
}

SpectrumAnalyzer::~SpectrumAnalyzer() {};

void SpectrumAnalyzer::execute_fft()
{
    std::vector<Complex> dft_input(input.cbegin(), input.cbegin() + input_size);

    std::vector<Complex> dft = PossDSP::ugly_fft(dft_input);

    output.clear();
    for (size_t i = 0; i < input_size; i++)
        output.push_back(std::norm(dft[i]));
}

static float dbFS(float amplitude);

void SpectrumAnalyzer::write(const char* name) const
{
    std::ofstream csv;
    csv.open(name);
    for (size_t i = 0; i < input_size / 2; i++)
    {
        csv << frequency_unit * i << ",";
    }
    csv << std::endl;
    for (size_t i = 0; i < input_size / 2; i++)
    {
        csv << dbFS(output[i]) << ",";
    }
    csv << std::endl;
    csv.close();
    std::cout << "fft csv written\n";
}

/*
start * x^n = end
x^n = end/start
x =
*/

void SpectrumAnalyzer::write_sampled(const char* name) const
{
    const float start_frequency = 20.0;
    const float end_frequency = 20000.0;
    const size_t n_bins = 200;

    const float multiplier = std::powf(end_frequency / start_frequency,
                                       1 / static_cast<float>(n_bins));

    std::ofstream csv;
    csv.open(name);

    float frequency = start_frequency;
    while (frequency < end_frequency)
    {
        csv << frequency << ',';
        frequency *= multiplier;
    }
    csv << '\n';

    frequency = start_frequency;
    while (frequency < end_frequency)
    {
        float amplitude = amplitude_at(frequency, output, input_sample_rate);
        csv << dbFS(amplitude) << ',';
        frequency *= multiplier;
    }

    csv.close();
}

static size_t crop_to_pow2(size_t sz)
{
    size_t power = 0;

    while ((1 << power) < sz)
        power++;

    power -= 1;
    return (1 << power);
}

static float dbFS(float amplitude)
{
    return 20.0f * (std::log10(amplitude) -
                    std::log10(std::numeric_limits<float>::max()));
}
