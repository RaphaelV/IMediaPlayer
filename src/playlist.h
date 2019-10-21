#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <filesystem>
#include <list>

namespace fs = std::filesystem;

class Playlist
{
public:
    Playlist();

    void play();
    void add(const fs::path& p);
    void clear();

    fs::path currentTrack();

    bool hasNext() const;
    bool hasPrevious() const;

    void next();
    void previous();

    bool empty() const {}

private:
    std::list<fs::path> m_tracks;
    mutable size_t index = 0;
};

#endif // PLAYLIST_H
