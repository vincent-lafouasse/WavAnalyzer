#pragma once

#include <vector>

struct SpectrumAnalyzer
{
    SpectrumAnalyzer(const std::vector<float>& signal, float sample_rate);
    ~SpectrumAnalyzer();

    void execute_fft();
    void execute_fftw3_NOT_MINE();
    void write(const char*) const;

    std::vector<double> input;
    std::vector<double> output;
    size_t input_size;
    float input_sample_rate;
    size_t first_bin;  // frequency offset, ignore below 20 Hz
    size_t n_bins;     // nnumber of frequency bins to fetch
};
