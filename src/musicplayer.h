#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <array>
#include <atomic>
#include <chrono>
#include <mutex>
#include <queue>
#include <string>

#include "track.h"

class MusicPlayer
{
public:
    enum class Command {
        Stop = 0,
        Play,
        Pause,
    };

    constexpr const char* displayCommand(Command command);

    enum class PlaybackState {
        Stopped = 0,
        Playing,
        Paused,
        Ended
    };

    constexpr const char*  displayPlaybackState(PlaybackState state) const;

    static constexpr std::array<const char*, 3> k_valid_track_ext = {".mp3d",".omg",".flaque"};
    static bool isTrackExtValid(const char* ext);

    PlaybackState playbackState() const;
    bool isPlaybackOver() const;

    std::chrono::milliseconds position() const;

    void playTrack(const Track& track);

    void request(Command command);

private:
    void setPlaybackState(PlaybackState command);
    void setPosition(std::chrono::milliseconds trackPosition);
    void trackPlaybackSimu(std::chrono::milliseconds duration);

    bool hasCommand() const;
    // Assume that there is a command in the queue
    Command readCommand();

private:
    std::atomic<PlaybackState> m_playback_state = PlaybackState::Stopped;
    std::atomic<std::chrono::milliseconds> m_position_ms = std::chrono::milliseconds{0};

    std::queue<Command> m_commands;
    mutable std::mutex m_commands_mutex;
};

#endif // MUSICPLAYER_H
