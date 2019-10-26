#ifndef IMEDIAPLAYER_H
#define IMEDIAPLAYER_H

#include <filesystem>
#include <optional>
#include <string>

#include "musicplayer.h"
#include "playlist.h"

namespace fs = std::filesystem;

class Track;

/* Main component the parse user commands, and
 * use the playlist to play the loaded tracks, with the music player. */
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
    // Called by the exec loop, play tracks one after the other
    void readPlaylist();
    void readCurrentTrack();

    bool isTrackValid(const fs::path& p);
    void showTrack(const fs::path& p);

    std::optional<Track> loadTrack(const fs::path& file_absolut_path);

private:
    bool m_run = true;
    MusicPlayer m_music_player;
    Playlist m_playlist;
};

#endif // IMEDIAPLAYER_H
