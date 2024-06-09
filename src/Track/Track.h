#pragma once

#include <vector>
#include "types.h"

struct TrackMetadata
{
    u16 n_channels;
    u32 sample_rate;
    u16 bit_depth;
    u32 data_size;
};

struct Track
{
    static Track from_wav(const char* path);
    void play_mono_blocking() const;
    u32 duration_ms() const;
    std::vector<float> left;
    std::vector<float> right;
    TrackMetadata metadata;
};
