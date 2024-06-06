#include <fstream>
#include <iostream>
#include <vector>

#include "FourCC.h"
#include "read.h"
#include "types.h"

enum class SampleFormat
{
    u8,
    i16,
    i24,
    i32,
};

SampleFormat bit_depth_to_format(u16 bit_depth)
{
    switch (bit_depth)
    {
        case 8:
            return SampleFormat::u8;
        case 16:
            return SampleFormat::i16;
        case 24:
            return SampleFormat::i24;
        case 32:
            return SampleFormat::i32;
        default:
            std::cout << "unrecognised bit depth " << bit_depth << '\n';
            exit(1);
    }
}

void log_sample_format(SampleFormat format)
{
    switch (format)
    {
        case SampleFormat::u8:
            std::cout << "u8\n";
            return;
        case SampleFormat::i16:
            std::cout << "i16\n";
            return;
        case SampleFormat::i24:
            std::cout << "i24\n";
            return;
        case SampleFormat::i32:
            std::cout << "i32\n";
            return;
    }
}

struct WavMetadata
{
    u16 n_channels;
    u32 sample_rate;
    SampleFormat sample_format;
};

const char* wav_path = "./wav/brk_upfront amen_1 bar_158 bpm.wav";

static void skip_chunk(const std::vector<Byte>& bytes, size_t& index)
{
    index += 4;  // skip tag
    u32 data_size = read_u32(bytes, index);
    index += data_size + (data_size % 2 == 1);
}

int main()
{
    std::ifstream input(wav_path, std::ios::binary);
    std::vector<Byte> bytes(std::istreambuf_iterator<char>(input), {});

    size_t index = 0;

    WavMetadata metadata;

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
    assert(sample_size * 8 == bit_depth * metadata.n_channels);

    metadata.sample_format = bit_depth_to_format(bit_depth);
    log_sample_format(metadata.sample_format);
}
