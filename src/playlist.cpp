#include <iostream>

#include "playlist.h"

Playlist::Playlist()
{

}

void Playlist::play()
{

}

void Playlist::add(const fs::path& p)
{
    m_tracks.push_back(p);
}

void Playlist::clear()
{
    std::cout << "Playlist cleared" << std::endl;
}

fs::path Playlist::currentTrack()
{
    if (m_tracks.size())
        return m_tracks.front();

    return {};
}

bool Playlist::hasNext() const
{
    return true;
}

bool Playlist::hasPrevious() const
{
    return false;
}

void Playlist::next()
{

}

void Playlist::previous()
{

}
