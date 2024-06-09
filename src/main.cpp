#include "Track/Track.h"

#include <iostream>

const char* wav_path = "./wav/brk_upfront amen_1 bar_158 bpm.wav";

struct SpectrogramParameters
{
    u32 frequency_resolution() const;
    u32 window_duration_ms() const;

    u32 sample_rate;
    u32 window_size;
    u32 time_increment;
};

u32 SpectrogramParameters::frequency_resolution() const
{
    return sample_rate / window_size;
}

u32 SpectrogramParameters::window_duration_ms() const
{
    return 1000 * window_size / static_cast<float>(sample_rate);
}

int main()
{
    Track track = Track::from_wav(wav_path);
    std::vector<float> signal = track.left;

    SpectrogramParameters spectrogram_parameters;
    spectrogram_parameters.sample_rate = track.metadata.sample_rate;
    spectrogram_parameters.window_size = 256;
    spectrogram_parameters.time_increment =
        spectrogram_parameters.window_size / 2;

    std::cout << "length of track "
              << 1000 * signal.size() /
                     static_cast<float>(track.metadata.sample_rate)
              << " ms\n";
    std::cout << "frequency resolution "
              << spectrogram_parameters.frequency_resolution() << '\n';
    std::cout << "window duration "
              << spectrogram_parameters.window_duration_ms() << " ms\n";
    std::cout << "number of points "
              << signal.size() / spectrogram_parameters.time_increment << "\n";
    return EXIT_SUCCESS;
}
