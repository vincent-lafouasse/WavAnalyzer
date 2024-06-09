#include "RawTrack.h"

#include "read.h"

#include <iostream>

static float sample_to_float(i64 sample, u8 sample_size);
static RawTrack parse_mono_track(const std::vector<Byte>& bytes,
                                 size_t start,
                                 TrackMetadata metadata);
static RawTrack parse_stereo_track(const std::vector<Byte>& bytes,
                                   size_t start,
                                   TrackMetadata metadata);

Track RawTrack::to_track()
{
    Track track;
    track.metadata = metadata;

    for (i64 sample : left)
        track.left.push_back(
            sample_to_float(sample, track.metadata.bit_depth / 8));

    for (i64 sample : right)
        track.right.push_back(
            sample_to_float(sample, track.metadata.bit_depth / 8));

    return track;
}

RawTrack RawTrack::from_bytes(const std::vector<Byte>& bytes,
                              size_t start,
                              TrackMetadata metadata)
{
    if (metadata.n_channels == 1)
        return parse_mono_track(bytes, start, metadata);
    if (metadata.n_channels == 2)
        return parse_stereo_track(bytes, start, metadata);

    std::cout << "can only parse mono and stereo data for now\n";
    exit(0);
}

static float sample_to_float(i64 sample, u8 sample_size)
{
    switch (sample_size)
    {
        case 1:
        {
            // unipolar -> float bipolar
            sample -= 128;
            // clamp
            if (sample <= -127)
                sample = -127;
            if (sample >= 127)
                sample = 127;
            return sample / 127.0;
        };
        case 3:
        {
            constexpr i64 i24_max = (1 << 23) - 1;
            // clamp
            if (sample <= -i24_max)
                sample = -i24_max;
            if (sample >= i24_max)
                sample = i24_max;
            return sample / static_cast<float>(i24_max);
        };
        default:
        {
            std::cout << "cant handle sample size " << sample_size << std::endl;
            std::exit(0);
        };
    }
}

static RawTrack parse_mono_track(const std::vector<Byte>& bytes,
                                 size_t start,
                                 TrackMetadata metadata)
{
    RawTrack track;
    track.metadata = metadata;

    const u8 sample_size = metadata.bit_depth / 8;
    const u32 n_samples = metadata.data_size / sample_size;

    for (u32 i = 0; i < n_samples; i++)
        track.left.push_back(
            read_sample(sample_size, bytes, start, IndexPolicy::Advance));
    track.right.clear();
    return track;
}

static RawTrack parse_stereo_track(const std::vector<Byte>& bytes,
                                   size_t start,
                                   TrackMetadata metadata)
{
    RawTrack track;
    track.metadata = metadata;

    const u8 sample_size = metadata.bit_depth / 8;
    const u32 n_samples = metadata.data_size / sample_size;

    for (u32 i = 0; i < n_samples; i++)
    {
        if (i % 2 == 0)
        {
            track.left.push_back(
                read_sample(sample_size, bytes, start, IndexPolicy::Advance));
        }
        else
        {
            track.right.push_back(
                read_sample(sample_size, bytes, start, IndexPolicy::Advance));
        }
    }

    return track;
}
