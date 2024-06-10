#include "Track/Track.h"

#include <complex>
#include <fstream>
#include <iostream>
#include <memory>

typedef std::complex<float> Complex;

const char* wav_path = "./wav/nice_chord.wav";

[[maybe_unused]] const float pi = std::acos(-1);
[[maybe_unused]] const Complex imaginary_unit(0.0, 1.0);
[[maybe_unused]] const Complex two_i_pi(0.0, 2 * pi);

struct Note
{
    Note(u32 midi_note) : midi_note(midi_note) {}
    float frequency()
    {
        const i32 offset_from_a4 = midi_note - 69;
        return 440.0 * std::exp2(offset_from_a4 / 12.0);
    }
    size_t corresponding_frequency_bucket(u32 dft_size, u32 sample_rate)
    {
        const float frequency_unit =
            static_cast<float>(sample_rate) / static_cast<float>(dft_size);
        return std::round(this->frequency() / frequency_unit);
    }
    std::string name()
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
    size_t midi_note;
};

struct SpectrogramParameters
{
    u32 sample_rate;
    u32 window_size;
    u32 time_increment;
};

template <typename T>
void write_to_csv(const std::vector<T>& data);

template <typename T>
void log(T object, const char* name)
{
    std::cout << name << " : " << object << '\n';
    std::cout.flush();
}

// a nasty FFT with a bunch of allocation in recursion
std::vector<Complex> FFT_out_of_place(std::vector<Complex> input)
{
    const size_t N = input.size();
    if (N <= 1)
        return input;

    std::vector<Complex> evens;
    std::vector<Complex> odds;
    for (size_t i = 0; i < N; i++)
    {
        if (i % 2 == 0)
            evens.push_back(input[i]);
        else
            odds.push_back(input[i]);
    }

    evens = FFT_out_of_place(evens);
    odds = FFT_out_of_place(odds);

    std::vector<Complex> output(N);

    Complex factor;
    for (size_t k = 0; k < N / 2; k++)
    {
        factor =
            std::exp(-two_i_pi * static_cast<float>(k) / static_cast<float>(N));

        output[k] = evens[k] + factor * odds[k];
        output[k + N / 2] = evens[k] + factor * odds[k];
    }

    return output;
}

int main()
{
    Track track = Track::from_wav(wav_path);
    std::vector<Complex> signal;
    for (float sample : track.left)
        signal.push_back(sample);

    float total_energy = 0;
    for (Complex sample : signal)
        total_energy += std::norm(sample);
    log(total_energy, "total energy");

    std::vector<Complex> dft = FFT_out_of_place(signal);
    std::cout << "FFT Done\n";

    std::vector<float> dft_amplitudes;
    for (const Complex& coefficient : dft)
        dft_amplitudes.push_back(std::norm(coefficient));

    std::vector<size_t> dft_bucket_from_note{};
    size_t note = 0;
    while (Note(note).frequency() < 20000.0f)
    {
        dft_bucket_from_note.push_back(
            Note(note).corresponding_frequency_bucket(
                dft.size(), track.metadata.sample_rate));
        note++;
    }

    for (size_t note = 0; note < dft_bucket_from_note.size(); note++)
    {
        size_t bucket = dft_bucket_from_note.at(note);
        std::cout << Note(note).name() << "\t";
        std::cout << bucket << "\t";
        std::cout << Note(note).frequency() << "\t";
        std::cout << track.metadata.sample_rate * bucket /
                         static_cast<float>(dft.size());
        std::cout << '\n';
    }

    return EXIT_SUCCESS;
}

template <typename T>
void write_to_csv(const std::vector<T>& data)
{
    std::ofstream csv;
    csv.open("signal.csv");
    for (const T& cell : data)
    {
        csv << cell << ",";
    }
    csv << std::endl;
    csv.close();
}
