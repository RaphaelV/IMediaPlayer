#ifndef IMEDIAPLAYER_H
#define IMEDIAPLAYER_H

#include <filesystem>
#include <optional>
#include <string>

#include "musicplayer.h"
#include "playlist.h"

namespace fs = std::filesystem;

class Track;

class IMediaPlayer
{
public:
    IMediaPlayer();

    void exec();
    void exit();

    void addTrack(const std::string& file);
    void removeTrack(const std::string& file);

    void play();
    void next();
    void previous();
    void pause();
    void stop();
    void trackPosition() const;

private:
    void parseCommand(const std::string& user_input);
    void readPlaylist();

    std::optional<Track> loadTrack(const fs::path& file_absolut_path);
    void removeTrack(const fs::path& file);

private:
    bool m_run = true;
    MusicPlayer m_music_player;
    Playlist m_playlist;
};

#endif // IMEDIAPLAYER_H
