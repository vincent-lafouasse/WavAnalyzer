#pragma once

#include "portaudio.h"

struct OutputStreamConfig
{
    static OutputStreamConfig default_config();
    int sample_rate;
    unsigned long buffer_size;
    int n_channels;
};

class OutputStream
{
   public:
    OutputStream();
    OutputStream(OutputStreamConfig stream_config);
    ~OutputStream();
    void start();
    void stop();

    void open(int (*callback)(const void* input_buffer,
                              void* output_buffer,
                              unsigned long buffer_size,
                              const PaStreamCallbackTimeInfo* time_info,
                              PaStreamCallbackFlags status_flags,
                              void* user_data),
              void* data);

    OutputStreamConfig cfg;

   private:
    int device;
    PaStreamParameters stream_params;
    PaStream* pa_stream;
};
