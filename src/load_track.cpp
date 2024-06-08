#include "Track.h"

#include <fstream>
#include "FourCC.h"
#include "read.h"

static void skip_chunk(const std::vector<Byte>& bytes, size_t& index)
{
    index += 4;  // skip tag
    u32 data_size = read_u32(bytes, index, IndexPolicy::Advance);
    index += data_size + (data_size % 2 == 1);
}

TrackMetadata parse_metadata(const std::vector<Byte>& bytes, size_t& index)
{
    TrackMetadata metadata;

    // riff chunk
    assert(fourcc_eq(read_four_cc(bytes, index, IndexPolicy::Advance), "RIFF"));
    assert(read_u32(bytes, index, IndexPolicy::Advance) + 8 == bytes.size());

    assert(fourcc_eq(read_four_cc(bytes, index, IndexPolicy::Advance), "WAVE"));

    while (!fourcc_eq(read_four_cc(bytes, index, IndexPolicy::Peek), "fmt "))
        skip_chunk(bytes, index);

    assert(fourcc_eq(read_four_cc(bytes, index, IndexPolicy::Advance), "fmt "));
    assert(read_u32(bytes, index, IndexPolicy::Advance) ==
           16);  // fmt chunk size
    constexpr u16 uncompressed_pcm = 1;
    assert(read_u16(bytes, index, IndexPolicy::Advance) == uncompressed_pcm);

    metadata.n_channels = read_u16(bytes, index, IndexPolicy::Advance);
    metadata.sample_rate = read_u32(bytes, index, IndexPolicy::Advance);

    read_u32(bytes, index, IndexPolicy::Advance);  // skip bitrate

    u16 sample_size = read_u16(bytes, index, IndexPolicy::Advance);
    u16 bit_depth = read_u16(bytes, index, IndexPolicy::Advance);
    assert((sample_size * 8 == bit_depth * metadata.n_channels) &&
           "cannot parse _exotic_ sample format (yet)");
    metadata.bit_depth = bit_depth;

    while (!fourcc_eq(read_four_cc(bytes, index, IndexPolicy::Peek), "data"))
        skip_chunk(bytes, index);

    assert(fourcc_eq(read_four_cc(bytes, index, IndexPolicy::Advance), "data"));
    metadata.data_size = read_u32(bytes, index, IndexPolicy::Advance);

    return metadata;
}

Track Track::from_wav(const char* path)
{
    std::ifstream input(path, std::ios::binary);

    std::vector<Byte> bytes(std::istreambuf_iterator<char>(input), {});
    size_t index = 0;

    TrackMetadata metadata = parse_metadata(bytes, index);

    RawTrack raw_track = RawTrack::from_bytes(bytes, index, metadata);

    return Track::from_raw_track(raw_track);
}
