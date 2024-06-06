#include <fstream>
#include <iostream>
#include <vector>

const char* wav_path = "../wav/brk_upfront amen_1 bar_158 bpm.wav";

int main()
{
    std::ifstream input(wav_path, std::ios::binary);
    std::vector<unsigned char> bytes(std::istreambuf_iterator<char>(input), {});
    std::cout << bytes.size() << " bytes\n";
}
