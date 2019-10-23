#include <iostream>
#include <set>
#include <sstream>

#include "utils.h"

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
    if (isFinished())
    {
        size_t nb_tracks = m_tracks.size();

        std::set<fs::path> track_already_founded;
        m_tracks.remove_if([&track_already_founded](const fs::path& p)
        {
            bool found = track_already_founded.find(p) != track_already_founded.end();
            track_already_founded.insert(p);
            return found;
        });

        std::cout << "Removed " << (nb_tracks - m_tracks.size()) << " track(s)" << std::endl;
    }
    else
    {
        std::cout << "Please stop before using removing duplicates" << std::endl;
    }
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
    if (m_random)
    {
        m_current_track = utils::getRandomElement(m_tracks.begin(), m_tracks.end());
        return;
    }

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
    if (m_random)
    {
        std::cout << "No previous track in random mode" << std::endl;
    }

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

void Playlist::toggleRepeat()
{
    m_repeat = !m_repeat;
    std::cout << "\t" << displayRepeat() << std::endl;
}

void Playlist::toggleRandom()
{
    m_random = !m_random;
    std::cout << "\t" << displayRandom() << std::endl;
}

std::string Playlist::displayRepeat() const
{
    std::stringstream ss;
    ss << "repeat: " << (m_repeat ? "ON" : "OFF");
    return ss.str();
}

std::string Playlist::displayRandom() const
{
    std::stringstream ss;
    ss << "random: " << (m_random ? "ON" : "OFF");
    return ss.str();
}

std::string Playlist::displayInfo() const
{
    std::stringstream ss;

    ss << "Playlist size:" << m_tracks.size() << "\n";
    ss << displayRepeat() << " "<< displayRandom() << "\n";

    for (const fs::path& track : m_tracks)
    {
        ss << track.filename() << "\n";
    }

    return ss.str();
}
