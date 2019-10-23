#include <iostream>
#include <sstream>

#include "playlist.h"


void Playlist::add(const fs::path& p)
{
    m_tracks.push_back(p);
}

void Playlist::remove(const fs::path& p)
{
    if (p != currentTrack())
        m_tracks.remove(p);
}

void Playlist::removeDuplicates()
{
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

void Playlist::next()
{
    if (hasNext())
    {
        m_current_track = std::next(m_current_track, 1);
    }
    else
    {
        m_current_track = m_repeat ? m_tracks.begin() : m_tracks.end();
    }
}

bool Playlist::hasPrevious() const
{
    return m_current_track != m_tracks.begin();
}

void Playlist::previous()
{
    if (hasPrevious())
    {
        m_current_track = std::prev(m_current_track);
    }
    else
    {
        if (m_repeat)
        {
            m_current_track = std::prev(m_tracks.end());
        }
    }
}

bool Playlist::empty() const
{
    return m_tracks.empty();
}

void Playlist::repeat(bool on)
{
    m_repeat = on;
}

void Playlist::toggleRepeat()
{
    m_repeat = !m_repeat;
    std::cout << "\t" << displayRepeat() << std::endl;
}

std::string Playlist::displayRepeat() const
{
    std::stringstream ss;
    ss << "repeat: " << (m_repeat ? "ON" : "OFF");
    return ss.str();
}

std::string Playlist::displayInfo() const
{
    std::stringstream ss;

    ss << "Playlist size:" << m_tracks.size() << "\n";
    ss << displayRepeat() << "\n";

    for (const fs::path& track : m_tracks)
    {
        ss << track.filename() << "\n";
    }

    return ss.str();
}
