#include <fstream>
#include <iostream>
#include <vector>

#include "FourCC.h"
#include "read.h"
#include "types.h"

struct SignalMetadata
{
    u16 n_channels;
    u32 sample_rate;
    u16 bit_depth;
    u32 data_size;
};

const char* wav_path = "./wav/brk_upfront amen_1 bar_158 bpm.wav";

static void skip_chunk(const std::vector<Byte>& bytes, size_t& index)
{
    index += 4;  // skip tag
    u32 data_size = read_u32(bytes, index);
    index += data_size + (data_size % 2 == 1);
}

i64 read_sample(const std::vector<Byte>& bytes, size_t start, u8 sample_size)
{
    if (sample_size == 1)
        return bytes[start];
    if (sample_size == 2)
    {
        i16 out = bytes[start] | bytes[start + 1] << 8;
        return static_cast<i64>(out);
    }
    return 0;
}

std::vector<i64> parse_first_channel(const std::vector<Byte>& bytes,
                                     size_t start,
                                     SignalMetadata metadata)
{
    const u8 sample_size = metadata.bit_depth / 8;
    const u32 channel_size = metadata.data_size / metadata.n_channels;
    std::cout << "sample size " << sample_size;
    std::cout << "\nchannel size: " << channel_size << '\n';
    std::vector<i64> channel;

    return channel;
}

int main()
{
    std::ifstream input(wav_path, std::ios::binary);
    std::vector<Byte> bytes(std::istreambuf_iterator<char>(input), {});

    size_t index = 0;

    SignalMetadata metadata;

    // riff chunk
    FourCC riff_tag = read_four_cc(bytes, index);
    assert(fourcc_eq(riff_tag, "RIFF"));
    assert(index == 4);
    assert(read_u32(bytes, index) + 8 == bytes.size());
    assert(index == 8);

    FourCC wave_tag = read_four_cc(bytes, index);
    assert(fourcc_eq(wave_tag, "WAVE"));
    assert(index == 12);

    skip_chunk(bytes, index);  // junk chunk
    skip_chunk(bytes, index);  // bext chunk
    skip_chunk(bytes, index);  // fake chunk

    FourCC fmt_tag = read_four_cc(bytes, index);
    assert(fourcc_eq(fmt_tag, "fmt "));
    assert(read_u32(bytes, index) == 16);  // fmt chunk size
    constexpr u16 uncompressed_pcm = 1;
    assert(read_u16(bytes, index) == uncompressed_pcm);

    metadata.n_channels = read_u16(bytes, index);
    std::cout << metadata.n_channels << " channels\n";

    metadata.sample_rate = read_u32(bytes, index);
    std::cout << "sample rate: " << metadata.sample_rate << '\n';

    read_u32(bytes, index);  // skip bitrate

    u16 sample_size = read_u16(bytes, index);
    u16 bit_depth = read_u16(bytes, index);
    std::cout << "sample size: " << sample_size;
    std::cout << "\nbit depth: " << bit_depth << '\n';
    assert((sample_size * 8 == bit_depth * metadata.n_channels) &&
           "cannot parse _exotic_ sample format (yet)");
    metadata.bit_depth = bit_depth;

    FourCC data_tag = read_four_cc(bytes, index);
    assert(fourcc_eq(data_tag, "data"));
    metadata.data_size = read_u32(bytes, index);

    std::cout << metadata.data_size << " bytes of data\n";

    std::vector<i64> raw_channel1 = parse_first_channel(bytes, index, metadata);
}
