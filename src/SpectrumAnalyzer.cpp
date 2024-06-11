#include "SpectrumAnalyzer.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include "fftw3.h"

static size_t crop_to_pow2(size_t sz);

SpectrumAnalyzer::SpectrumAnalyzer(const std::vector<float>& signal,
                                   float sample_rate)
{
    input_size = crop_to_pow2(signal.size());
    input = std::vector<double>(signal.cbegin(), signal.cbegin() + input_size);
    output = std::vector<double>(input_size);

    input_sample_rate = sample_rate;
    const double frequency_unit = input_sample_rate / input_size;

    first_bin = std::ceil(20.0 / frequency_unit);

    const size_t last_bin = std::min(
        input_size, static_cast<size_t>(std::ceil(20000 / frequency_unit)));
    n_bins = last_bin - first_bin;
}

SpectrumAnalyzer::~SpectrumAnalyzer() {};

void SpectrumAnalyzer::execute_fftw3_NOT_MINE()
{
    const unsigned int plan_flags = FFTW_ESTIMATE;
    const fftw_r2r_kind kind = FFTW_HC2R;
    fftw_plan plan = fftw_plan_r2r_1d(input_size, &(input[0]), &(output[0]),
                                      kind, plan_flags);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
}
void SpectrumAnalyzer::write()
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

static size_t crop_to_pow2(size_t sz)
{
    size_t power = 0;

    while ((1 << power) < sz)
        power++;

    power -= 1;
    return (1 << power);
}
