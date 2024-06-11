#include "SpectrumAnalyzer.h"

#include "fftw3.h"

void SpectrumAnalyzer::execute_fftw3_NOT_MINE()
{
    const unsigned int plan_flags = FFTW_ESTIMATE;
    const fftw_r2r_kind kind = FFTW_HC2R;
    fftw_plan plan = fftw_plan_r2r_1d(input_size, &(input[0]), &(output[0]),
                                      kind, plan_flags);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
}
