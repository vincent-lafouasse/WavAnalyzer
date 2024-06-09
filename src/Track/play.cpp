#include "OutputStream/OutputStream.h"
#include "Track.h"

#include <chrono>
#include <thread>
#include "portaudio.h"

struct PlaybackData
{
    PlaybackData(const std::vector<float>& samples) : samples(samples), index(0)
    {
    }
    const std::vector<float>& samples;
    size_t index;
};

// audio callback
static int playback(const void* input_buffer,
                    void* output_buffer,
                    unsigned long buffer_size,
                    const PaStreamCallbackTimeInfo* time_info,
                    PaStreamCallbackFlags status_flags,
                    void* user_data);

void Track::play_mono_blocking() const
{
    constexpr u32 buffer_size = 512;
    OutputStreamConfig cfg = {static_cast<int>(metadata.sample_rate),
                              buffer_size, 1};
    OutputStream stream(cfg);

    PlaybackData data(this->left);
    stream.open(playback, &data);
    stream.start();

    auto playback_duration =
        std::chrono::milliseconds(this->duration_ms() + 250);
    std::this_thread::sleep_for(playback_duration);

    stream.stop();
}

static int playback(const void* input_buffer,
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
