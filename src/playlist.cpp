#include <iostream>

#include "playlist.h"

void Playlist::add(const fs::path& p)
{
    m_tracks.push_back(p);
}

void Playlist::clear()
{
    m_tracks.clear();
    m_current_track = m_tracks.end();
    std::cout << "Playlist cleared" << std::endl;
}

void Playlist::play()
{
    if (empty())
    {
        std::cout << "The playlist is empty" << std::endl;
        return;
    }

    if (isFinished())
        m_current_track = m_tracks.begin();
}

void Playlist::stop()
{
    m_current_track = m_tracks.end();
}

bool Playlist::isFinished() const
{
    return m_current_track == m_tracks.end();
}

fs::path Playlist::currentTrack() const
{
    if (isFinished())
        return {};

    return *m_current_track;
}

bool Playlist::hasNext() const
{
    if (isFinished() || std::next(m_current_track) == m_tracks.end())
        return false;

    return true;
}

fs::path Playlist::next()
{
    if (isFinished())
        return {};

    m_current_track = std::next(m_current_track, 1);

    if (isFinished())
        return {};

    return  *m_current_track;
}

bool Playlist::hasPrevious() const
{
   return m_current_track != m_tracks.begin();
}

fs::path Playlist::previous()
{
    if (!hasPrevious())
        return {};

    m_current_track = std::prev(m_current_track, 1);
    return  *m_current_track;
}

bool Playlist::empty() const
{
    return m_tracks.empty();
}
