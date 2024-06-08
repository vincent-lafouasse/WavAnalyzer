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
    std::vector<float> left;
    std::vector<float> right;
    TrackMetadata metadata;
};

struct RawTrack
{
    static RawTrack from_bytes(const std::vector<Byte>& bytes,
                               size_t start,
                               TrackMetadata metadata);
    Track to_track();
    std::vector<i64> left;
    std::vector<i64> right;
    TrackMetadata metadata;
};
