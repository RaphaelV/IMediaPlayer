#ifndef IMEDIAPLAYER_H
#define IMEDIAPLAYER_H

#include <array>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class IMediaPlayer
{
public:
    IMediaPlayer();

    void exec();

    static fs::path convertToAbsolutePath(const std::string& file);
    void play(const fs::path& file);
    void resume();

    static constexpr std::array<const char*, 3> k_valid_track_ext = {".mp3d",".omg",".flaque"};
    static constexpr const char* k_valid_playlist_ext = ".plst";

private:
    static bool isTrackExtValid(const char* ext);

private:
    uint64_t m_position_ms = 0;
};

#endif // IMEDIAPLAYER_H
