#include "Track/Track.h"

#define TESTING 0
#if TESTING
#include "test/test.h"
#endif

const char* wav_path = "./wav/hat_short.wav";

int main()
{
#if TESTING
    run_tests_and_exit();
#endif

    Track track = Track::from_wav(wav_path);
}
