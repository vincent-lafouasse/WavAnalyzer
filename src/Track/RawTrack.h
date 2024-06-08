#pragma once

#include "Track.h"

struct RawTrack
{
    static RawTrack from_bytes(const std::vector<Byte>& bytes,
                               size_t start,
                               TrackMetadata metadata);
    Track to_track();
    std::vector<i64> left;
    std::vector<i64> right;
    TrackMetadata metadata;
};
