#ifndef IMEDIAPLAYER_H
#define IMEDIAPLAYER_H

#include <array>

class IMediaPlayer
{
public:
    IMediaPlayer();

    void exec();

    static constexpr std::array<const char*, 3> k_valid_track_ext = {"mp3d","omg","flaque"};
    static constexpr const char* k_valid_playlist_ext = "playlist";

private:
    uint64_t m_position_ms = 0;
};

#endif // IMEDIAPLAYER_H
