#include "OutputStream.h"

#include <cstdlib>
#include <iostream>

static void check_error(PaError err);
static int query_output_device(int n_channels);

OutputStream::OutputStream(OutputStreamConfig stream_config)
{
    check_error(Pa_Initialize());
    cfg = stream_config;
    device = query_output_device(cfg.n_channels);

    std::memset(&stream_params, 0, sizeof(stream_params));
    stream_params.channelCount = cfg.n_channels;
    stream_params.device = device;
    stream_params.hostApiSpecificStreamInfo = NULL;
    stream_params.sampleFormat = paFloat32;
    stream_params.suggestedLatency =
        Pa_GetDeviceInfo(device)->defaultLowOutputLatency;
}

void OutputStream::open(
    int (*callback)(const void* input_buffer,
                    void* output_buffer,
                    unsigned long buffer_size,
                    const PaStreamCallbackTimeInfo* time_info,
                    PaStreamCallbackFlags status_flags,
                    void* user_data),
    void* data)
{
    check_error(Pa_OpenStream(&pa_stream, NULL, &stream_params, cfg.sample_rate,
                              cfg.buffer_size, paNoFlag, callback, data));
}

void OutputStream::start()
{
    check_error(Pa_StartStream(pa_stream));
}

void OutputStream::stop()
{
    check_error(Pa_StopStream(pa_stream));
}

OutputStream::~OutputStream()
{
    check_error(Pa_CloseStream(pa_stream));
    check_error(Pa_Terminate());
}

static int get_n_devices();
static void log_devices(int n_devices);

static int query_output_device(int n_channels)
{
    int n_devices = get_n_devices();
    log_devices(n_devices);

    int device;
    while (1)
    {
        std::cout << "Which output device do you choose?\n";
        std::cin >> device;

        if (device >= n_devices || device < 0)
        {
            std::cout
                << "device not found, please select a device between 0 and "
                << n_devices - 1 << '\n';
            continue;
        }
        const PaDeviceInfo* info = Pa_GetDeviceInfo(device);
        if (info->maxOutputChannels < n_channels)
        {
            std::cout
                << "this device does not provide enough output channels\n";
            continue;
        }
        break;
    }

    return device;
}

static int get_n_devices()
{
    int n_devices = Pa_GetDeviceCount();
    std::cout << "number of devices: " << n_devices << '\n';
    if (n_devices < 0)
    {
        std::cout << "Error getting device count\n";
        exit(EXIT_FAILURE);
    }
    else if (n_devices == 0)
    {
        std::cout << "No audio device found\n";
        exit(EXIT_SUCCESS);
    }
    return n_devices;
}

static void log_devices(int n_devices)
{
    for (int i = 0; i < n_devices; i++)
    {
        const PaDeviceInfo* device_info = Pa_GetDeviceInfo(i);
        std::cout << "Device " << i << '\n';
        std::cout << "\tname " << device_info->name << '\n';
        std::cout << "\tmax input channels " << device_info->maxOutputChannels
                  << '\n';
        std::cout << "\tmax output channels " << device_info->maxOutputChannels
                  << '\n';
        std::cout << "\tdefault sample rate " << device_info->defaultSampleRate
                  << "\n\n";
    }
}

static void check_error(PaError err)
{
    if (err != paNoError)
    {
        std::cout << "Portaudio error: " << Pa_GetErrorText(err) << '\n';
        exit(EXIT_FAILURE);
    }
}
