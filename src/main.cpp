#include "Track/Track.h"
#include "portaudio.h"

#include <iostream>

const char* wav_path = "./wav/brk_upfront amen_1 bar_158 bpm.wav";

struct PlaybackData
{
    PlaybackData(std::vector<float>& samples) : samples(samples), index(0) {}
    std::vector<float>& samples;
    size_t index;
};

int playback(const void* input_buffer,
             void* output_buffer,
             unsigned long buffer_size,
             const PaStreamCallbackTimeInfo* time_info,
             PaStreamCallbackFlags status_flags,
             void* user_data)
{
    (void)input_buffer;
    (void)time_info;
    (void)status_flags;

    PlaybackData* data = static_cast<PlaybackData*>(user_data);
    float* output = static_cast<float*>(output_buffer);

    for (size_t i = 0; i < buffer_size; i++)
    {
        output[i] =
            (i < data->samples.size()) ? data->samples[data->index] : 0.0;
        data->index += 1;
    }

    return 0;
}

int track_duration_ms(const Track& track)
{
    return 250 + 1000 * static_cast<float>(track.left.size()) /
                     static_cast<float>(track.metadata.sample_rate);
}

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

    SpectrogramParameters spectrogram_parameters;
    spectrogram_parameters.sample_rate = track.metadata.sample_rate;
    spectrogram_parameters.window_size = 64;

    std::cout << "frequency resolution "
              << spectrogram_parameters.frequency_resolution() << '\n';
    std::cout << "window duration "
              << spectrogram_parameters.window_duration_ms() << " ms\n";

    return EXIT_SUCCESS;
}
