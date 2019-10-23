#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <filesystem>
#include <list>

namespace fs = std::filesystem;

class Playlist
{
public:
    void add(const fs::path& p);
    void remove(const fs::path& p);
    void removeDuplicates();
    void clear();

    void play();
    void stop();

    bool isFinished() const;
    // Returns an empty path if there is no current track
    fs::path currentTrack() const;

    bool hasNext() const;
    void next();

    bool hasPrevious() const;
    void previous();

    bool empty() const;

    void repeat(bool on);
    void toggleRepeat();

private:
    std::list<fs::path> m_tracks;
    std::list<fs::path>::iterator m_current_track = m_tracks.begin();

    bool m_repeat = false;
};

#endif // PLAYLIST_H
