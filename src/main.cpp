#include <fstream>
#include <iostream>
#include <vector>

#include "FourCC.h"
#include "read.h"
#include "types.h"

#define TESTING 0
#if TESTING
#include "test/test.h"
#endif

struct TrackMetadata
{
    u16 n_channels;
    u32 sample_rate;
    u16 bit_depth;
    u32 data_size;
};

struct RawTrack
{
    std::vector<i64> left;
    std::vector<i64> right;
    TrackMetadata metadata;
};

const char* wav_path = "./wav/brk_upfront amen_1 bar_158 bpm.wav";

static void skip_chunk(const std::vector<Byte>& bytes, size_t& index)
{
    index += 4;  // skip tag
    u32 data_size = read_u32(bytes, index, IndexPolicy::Advance);
    index += data_size + (data_size % 2 == 1);
}

RawTrack parse_mono_track(const std::vector<Byte>& bytes,
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

RawTrack parse_stereo_track(const std::vector<Byte>& bytes,
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

RawTrack parse_raw_track(const std::vector<Byte>& bytes,
                         size_t start,
                         TrackMetadata metadata)
{
    const u8 sample_size = metadata.bit_depth / 8;
    const u32 channel_size = metadata.data_size / metadata.n_channels;
    std::cout << "sample size " << sample_size;
    std::cout << "\nchannel size: " << channel_size << '\n';

    if (metadata.n_channels == 1)
        return parse_mono_track(bytes, start, metadata);
    if (metadata.n_channels == 2)
        return parse_stereo_track(bytes, start, metadata);

    std::cout << "can only parse mono and stereo data for now\n";
    exit(0);
}

int main()
{
#if TESTING
    run_tests_and_exit();
#endif

    std::ifstream input(wav_path, std::ios::binary);
    std::vector<Byte> bytes(std::istreambuf_iterator<char>(input), {});

    size_t index = 0;

    TrackMetadata metadata;

    // riff chunk
    FourCC riff_tag = read_four_cc(bytes, index, IndexPolicy::Advance);
    assert(fourcc_eq(riff_tag, "RIFF"));
    assert(index == 4);
    assert(read_u32(bytes, index, IndexPolicy::Advance) + 8 == bytes.size());
    assert(index == 8);

    FourCC wave_tag = read_four_cc(bytes, index, IndexPolicy::Advance);
    assert(fourcc_eq(wave_tag, "WAVE"));
    assert(index == 12);

    skip_chunk(bytes, index);  // junk chunk
    skip_chunk(bytes, index);  // bext chunk
    skip_chunk(bytes, index);  // fake chunk

    FourCC fmt_tag = read_four_cc(bytes, index, IndexPolicy::Advance);
    assert(fourcc_eq(fmt_tag, "fmt "));
    assert(read_u32(bytes, index, IndexPolicy::Advance) ==
           16);  // fmt chunk size
    constexpr u16 uncompressed_pcm = 1;
    assert(read_u16(bytes, index, IndexPolicy::Advance) == uncompressed_pcm);

    metadata.n_channels = read_u16(bytes, index, IndexPolicy::Advance);
    std::cout << metadata.n_channels << " channels\n";

    metadata.sample_rate = read_u32(bytes, index, IndexPolicy::Advance);
    std::cout << "sample rate: " << metadata.sample_rate << '\n';

    read_u32(bytes, index, IndexPolicy::Advance);  // skip bitrate

    u16 sample_size = read_u16(bytes, index, IndexPolicy::Advance);
    u16 bit_depth = read_u16(bytes, index, IndexPolicy::Advance);
    std::cout << "sample size: " << sample_size;
    std::cout << "\nbit depth: " << bit_depth << '\n';
    assert((sample_size * 8 == bit_depth * metadata.n_channels) &&
           "cannot parse _exotic_ sample format (yet)");
    metadata.bit_depth = bit_depth;

    FourCC data_tag = read_four_cc(bytes, index, IndexPolicy::Advance);
    assert(fourcc_eq(data_tag, "data"));
    metadata.data_size = read_u32(bytes, index, IndexPolicy::Advance);

    std::cout << metadata.data_size << " bytes of data\n";

    if (metadata.n_channels > 2)
    {
        std::cout << "can only parse mono and stereo data for now\n";
        exit(0);
    }

    RawTrack raw_track = parse_raw_track(bytes, index, metadata);
    std::cout << raw_track.left.size() << " ";
    std::cout << raw_track.right.size() << " ";
}
