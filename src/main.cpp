#include "Track/Track.h"

#include <iostream>
#include <numeric>

const char* wav_path = "./wav/nice_chord.wav";

struct SpectrogramParameters
{
    u32 frequency_resolution() const;
    u32 window_duration_ms() const;

    u32 sample_rate;
    u32 window_size;
    u32 time_increment;
};

template <typename T>
void log(T object, const char* name)
{
    std::cout << name << " :\t" << object << '\n';
    std::cout.flush();
}

int main()
{
    Track track = Track::from_wav(wav_path);
    track.play_mono_blocking();
    std::vector<float> signal = track.left;

    float total_energy = std::inner_product(signal.cbegin(), signal.cend(),
                                            signal.cbegin(), 0.0);
    log(total_energy, "total energy");

    return EXIT_SUCCESS;
}

u32 SpectrogramParameters::frequency_resolution() const
{
    return sample_rate / window_size;
}

u32 SpectrogramParameters::window_duration_ms() const
{
    return 1000 * window_size / static_cast<float>(sample_rate);
}
