#pragma once

#include <array>
#include <cstdint>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef std::size_t usize;

typedef u8 Byte;

struct Int24
{
    i64 to_i64() const;
    double to_double() const;

    std::array<Byte, 3> little_endian_bytes;
};
