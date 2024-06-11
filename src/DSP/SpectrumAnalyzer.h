#pragma once

#include <vector>

struct SpectrumAnalyzer
{
    SpectrumAnalyzer(const std::vector<float>& signal, float sample_rate);
    ~SpectrumAnalyzer();

    void execute_fft();
    void execute_fftw3_NOT_MINE();
    void write(const char*) const;
    void write_sampled(const char*) const;

    std::vector<double> input;
    std::vector<double> output;
    size_t input_size;
    float input_sample_rate;
    float frequency_unit;
};
