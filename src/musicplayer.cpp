#include <iostream>
#include <thread>

#include "utils.h"

#include "musicplayer.h"

std::string MusicPlayer::displayCommand(Command command) const
{
    if (command == Command::Play)
        return "Playing";

    if (command == Command::Pause)
        return "Paused";

    if (command == Command::Stop)
        return "Stopped";

    return "";
}

bool MusicPlayer::isTrackExtValid(const char* ext)
{
    auto it = std::find_if(k_valid_track_ext.begin(), k_valid_track_ext.end(),
                           [ext](const char* valid_ext)
    {
        return std::strcmp(ext, valid_ext);
    });

    return it != k_valid_track_ext.end();
}

std::chrono::milliseconds MusicPlayer::position() const
{
    return m_position_ms;
}

void MusicPlayer::playTrack(const Track &track)
{
    std::cout << "\n\n" << track.displayInfo() << std::endl;
    auto d = track.duration();
    std::thread t{[this, d]() { trackPlaybackSimu(d); }};
    t.detach();
}

void MusicPlayer::setPosition(std::chrono::milliseconds pos)
{
    m_position_ms = pos;
}

MusicPlayer::Command MusicPlayer::command() const
{
    return  m_command;
}

void MusicPlayer::setCommand(MusicPlayer::Command command)
{
    m_command = command;
}

MusicPlayer::PlaybackState MusicPlayer::playbackState() const
{
    return m_playback_state;
}

void MusicPlayer::setPlaybackState(PlaybackState state)
{
    m_playback_state = state;
}

void MusicPlayer::trackPlaybackSimu(std::chrono::milliseconds duration)
{
    setPlaybackState(PlaybackState::Busy);
    setPosition(std::chrono::milliseconds{0});

    auto update_command = [this](Command& local_command)
    {
        if (local_command != command())
        {
            local_command = command();
        }
    };

    Command command = Command::Stop;
    update_command(command);

    for (;command != Command::Stop; update_command(command))
    {
        if (command == Command::Play)
        {
            auto one_sec = std::chrono::seconds{1};
            // Sound playing simulation
            std::this_thread::sleep_for(one_sec);
            auto p = position() + one_sec;

            if (p > duration)
                break;

            setPosition(p);
        }
        else if (command == Command::Pause)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{200});
        }
    }

    setPlaybackState(PlaybackState::Ready);
}
