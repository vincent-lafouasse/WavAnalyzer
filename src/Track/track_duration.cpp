#include "Track.h"

u32 Track::duration_ms() const
{
    return 1000 * static_cast<float>(left.size()) /
           static_cast<float>(metadata.sample_rate);
}
