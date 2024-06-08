#include "OutputStream/OutputStream.h"
#include "Track/Track.h"
#include "portaudio.h"

#include <thread>

const char* wav_path = "./wav/brk_upfront amen_1 bar_158 bpm.wav";
constexpr size_t sample_rate = 48000;
constexpr size_t buffer_size = 512;
constexpr size_t n_channels = 1;

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

int main()
{
    Track track = Track::from_wav(wav_path);

    constexpr OutputStreamConfig cfg = {sample_rate, buffer_size, n_channels};
    OutputStream stream(cfg);

    PlaybackData data(track.left);
    stream.open(&playback, &data);
    stream.start();

    auto capture_duration = std::chrono::milliseconds(track_duration_ms(track));
    std::this_thread::sleep_for(capture_duration);

    stream.stop();

    return EXIT_SUCCESS;
}
