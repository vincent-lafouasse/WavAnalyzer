#pragma once

#include "types.h"

#include <string>

struct Note
{
    Note(u32 midi_note) : midi_note(midi_note) {}
    float frequency();
    size_t corresponding_frequency_bucket(u32 dft_size, u32 sample_rate);
    std::string name();

    size_t midi_note;
};
