#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "cool_ints.h"

const char* wav_path = "./wav/brk_upfront amen_1 bar_158 bpm.wav";

typedef unsigned char Byte;
typedef std::array<Byte, 4> FourCC;

static bool fourcc_eq(FourCC four_cc, const char* pattern)
{
    for (size_t i = 0; i < 4 && pattern[i]; i++)
    {
        if (four_cc[i] != pattern[i])
            return false;
    }
    return true;
}

static u32 read_u32(const std::vector<Byte>& bytes, size_t& index)
{
    assert(index + 4 < bytes.size());

    u32 out = 0;
    for (size_t _ = 0; _ < 4; _++, index++)
    {
        out = (out << 8) + static_cast<u32>(bytes[index]);
    }
    return out;

}

static FourCC read_four_cc(const std::vector<Byte>& bytes, size_t& index)
{
    assert(index + 4 < bytes.size());

    FourCC out;
    for (size_t i = 0; i < 4; i++, index++)
    {
        out[i] = bytes[index];
    }
    return out;
}

int main()
{
    std::ifstream input(wav_path, std::ios::binary);
    std::vector<Byte> bytes(std::istreambuf_iterator<char>(input), {});

    size_t index = 0;

    FourCC wave_tag = read_four_cc(bytes, index);
    assert(fourcc_eq(wave_tag, "RIFF"));
    assert(index == 4);

    u32 file_size = read_u32(bytes, index) + 8;
    assert(index ==  8);
    std::cout << file_size / 64 << std::endl;
    std::cout << bytes.size() << std::endl;
}
