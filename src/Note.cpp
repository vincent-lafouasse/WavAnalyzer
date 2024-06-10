#include "Note.h"

#include <cmath>
#include <iostream>

size_t Note::corresponding_frequency_bucket(u32 dft_size, u32 sample_rate)
{
    const float frequency_unit =
        static_cast<float>(sample_rate) / static_cast<float>(dft_size);
    return std::round(this->frequency() / frequency_unit);
}

float Note::frequency()
{
    const i32 offset_from_a4 = midi_note - 69;
    return 440.0 * std::exp2(offset_from_a4 / 12.0);
}

std::string Note::name()
{
    int octave = (midi_note / 12) - 1;
    int notename = midi_note % 12;
    switch (notename)
    {
        case 0:
            return "C" + std::to_string(octave);
        case 1:
            return "C#/Db" + std::to_string(octave);
        case 2:
            return "D" + std::to_string(octave);
        case 3:
            return "Eb/D#" + std::to_string(octave);
        case 4:
            return "E" + std::to_string(octave);
        case 5:
            return "F" + std::to_string(octave);
        case 6:
            return "F#/Gb" + std::to_string(octave);
        case 7:
            return "G" + std::to_string(octave);
        case 8:
            return "Ab/G#" + std::to_string(octave);
        case 9:
            return "A" + std::to_string(octave);
        case 10:
            return "Bb/A#" + std::to_string(octave);
        case 11:
            return "B" + std::to_string(octave);
        default:
            std::cout << "unreachable";
            exit(1);
    }
}
