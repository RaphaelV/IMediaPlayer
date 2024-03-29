#include <cassert>
#include <iostream>
#include <thread>

#include "utils.h"

#include "musicplayer.h"


constexpr const char* MusicPlayer::displayCommand(MusicPlayer::Command command)
{
    if (command == Command::Play)
        return "Play";

    if (command == Command::Pause)
        return "Pause";

    if (command == Command::Stop)
        return "Stop";

    return "";
}

constexpr const char* MusicPlayer::displayPlaybackState(MusicPlayer::PlaybackState state) const
{
    if (state == PlaybackState::Playing)
        return "Playing";

    if (state == PlaybackState::Paused)
        return "Paused";

    if (state == PlaybackState::Stopped)
        return "Stopped";

    if (state == PlaybackState::Ended)
        return "Ended";

    return "";
}

bool MusicPlayer::isTrackExtValid(const char* ext)
{
    auto it = std::find_if(k_valid_track_ext.begin(), k_valid_track_ext.end(),
                           [ext](const char* valid_ext)
    {
        return std::strcmp(ext, valid_ext) == 0;
    });

    return it != k_valid_track_ext.end();
}

std::chrono::milliseconds MusicPlayer::position() const
{
    return m_position_ms;
}

void MusicPlayer::playTrack(const Track& track)
{
    if (isPlaybackOver())
    {
        auto d = track.duration();
        setPosition(std::chrono::milliseconds{0});
        // Simulate a service that reads music data,
        // (raw music data could be added in parameters)
        std::thread t{[this, d]() { trackPlaybackSimu(d); }};
        t.detach();
    }
}

void MusicPlayer::request(MusicPlayer::Command command)
{
    std::lock_guard<std::mutex> lock(m_commands_mutex);
    m_commands.push(command);
}

void MusicPlayer::setPosition(std::chrono::milliseconds pos)
{
    m_position_ms = pos;
}

MusicPlayer::PlaybackState MusicPlayer::playbackState() const
{
    return m_playback_state;
}

bool MusicPlayer::isPlaybackOver() const
{
    return playbackState() == PlaybackState::Stopped ||
            playbackState() == PlaybackState::Ended;
}

void MusicPlayer::setPlaybackState(PlaybackState state)
{
    if (m_playback_state == state)
        return;

    m_playback_state = state;
    std::cout << std::right << std::setw(70) << "Track: " << displayPlaybackState(state) << std::endl;
}

void MusicPlayer::trackPlaybackSimu(std::chrono::milliseconds duration)
{
    Command command = Command::Play;

    while (command != Command::Stop)
    {
        if (command == Command::Play)
        {
            setPlaybackState(PlaybackState::Playing);
            auto read_time = std::chrono::milliseconds{500};
            // Sound playing simulation
            std::this_thread::sleep_for(read_time);
            auto p = position() + read_time;

            // Playback duration can be longer than read time
            if (p > duration)
            {
                setPlaybackState(PlaybackState::Ended);
                return;
            }

            setPosition(p);
        }
        else
        {
            setPlaybackState(PlaybackState::Paused);
            std::this_thread::sleep_for(std::chrono::milliseconds{200});
        }

        if (hasCommand())
        {
            command = readCommand();
        }
    }

    setPlaybackState(PlaybackState::Stopped);
}

bool MusicPlayer::hasCommand() const
{
    std::unique_lock<std::mutex> lock(m_commands_mutex);
    return !m_commands.empty();
}

MusicPlayer::Command MusicPlayer::readCommand()
{
    std::unique_lock<std::mutex> lock(m_commands_mutex);
    assert(!m_commands.empty());

    Command command = m_commands.front();
    m_commands.pop();

    return command;
}
