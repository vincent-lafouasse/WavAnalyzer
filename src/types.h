#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef unsigned char Byte;

class Int24
{
   public:
    static Int24 read_from_bytes(const std::vector<Byte>& bytes, size_t index);
    i64 to_i64() const;
    double to_double() const;

   private:
    Byte internals[3];
};
