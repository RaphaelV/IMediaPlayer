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
    size_t nbTracks() const;

    void toggleRepeat();
    void toggleRandom();

    std::string displayRepeat() const;
    std::string displayRandom() const;
    std::string displayInfo() const;

private:
    std::list<fs::path> m_tracks;
    std::list<fs::path>::iterator m_current_track_it = m_tracks.begin();

    bool m_repeat = false;
    bool m_random = false;
};

#endif // PLAYLIST_H
