#include "OutputStream/OutputStream.h"
#include "Track/Track.h"
#include "portaudio.h"

#include <iostream>
#include <thread>

const char* wav_path = "./wav/hat_short.wav";
constexpr size_t sample_rate = 48000;
constexpr size_t buffer_size = 512;

int do_nothing_callback(const void* input_buffer,
                        void* output_buffer,
                        unsigned long buffer_size,
                        const PaStreamCallbackTimeInfo* time_info,
                        PaStreamCallbackFlags status_flags,
                        void* user_data);

int main()
{
    Track track = Track::from_wav(wav_path);

    constexpr OutputStreamConfig cfg = {sample_rate, buffer_size, 2};

    OutputStream stream(cfg);
    stream.open(&do_nothing_callback, NULL);
    stream.start();

    auto capture_duration = std::chrono::seconds(10);
    std::this_thread::sleep_for(capture_duration);

    stream.stop();

    return EXIT_SUCCESS;
}

int do_nothing_callback(const void* input_buffer,
                        void* output_buffer,
                        unsigned long buffer_size,
                        const PaStreamCallbackTimeInfo* time_info,
                        PaStreamCallbackFlags status_flags,
                        void* user_data)
{
    (void)input_buffer;
    (void)output_buffer;
    (void)buffer_size;
    (void)time_info;
    (void)status_flags;
    (void)user_data;
    std::cout << 'a';
    std::cout.flush();
    return 0;
}
