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
        size_t nb_tracks = nbTracks();

        std::set<fs::path> track_already_founded;
        m_tracks.remove_if([&track_already_founded](const fs::path& p)
        {
            bool found = track_already_founded.find(p) != track_already_founded.end();
            track_already_founded.insert(p);
            return found;
        });

        std::cout << "\tRemoved " << (nb_tracks - nbTracks()) << " track(s)\n" << std::endl;
    }
    else
    {
        std::cout << "\tPlease stop before using removing duplicates\n" << std::endl;
    }
}

void Playlist::clear()
{
    m_tracks.clear();
    m_current_track = m_tracks.end();
    std::cout << "\tPlaylist cleared\n" << std::endl;
}

void Playlist::play()
{
    if (empty())
    {
        std::cout << "\tThe playlist is empty\n" << std::endl;
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

        if (isFinished())
            std::cout << "\tPlaylist finished\n" << std::endl;
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
        std::cout << "\tNo previous track in random mode\n" << std::endl;
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

size_t Playlist::nbTracks() const
{
    return m_tracks.size();
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

    ss << "\tPlaylist size:" << nbTracks() << "\n";
    ss << "\t" << displayRepeat() << " "<< displayRandom() << "\n";

    size_t track_indice = 0;

    for (const fs::path& track : m_tracks)
    {
        ss << "\t" << (++track_indice) << " - " << track.filename() << (track == currentTrack() ? " <" :"") << "\n";
    }

    return ss.str();
}
