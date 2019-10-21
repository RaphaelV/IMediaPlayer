#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <array>
#include <atomic>
#include <chrono>
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
    std::string displayCommand(Command command) const;

    enum class PlaybackState {
        Ready = 0,
        Busy
    };

    static constexpr std::array<const char*, 3> k_valid_track_ext = {".mp3d",".omg",".flaque"};
    static bool isTrackExtValid(const char* ext);

    Command command() const;
    void setCommand(Command command);

    PlaybackState playbackState() const;

    std::chrono::milliseconds position() const;

    void playTrack(const Track& track);

private:
    void setPlaybackState(PlaybackState command);
    void setPosition(std::chrono::milliseconds trackPosition);
    void trackPlaybackSimu(std::chrono::milliseconds duration);

private:
    std::atomic<Command> m_command = Command::Stop;
    std::atomic<PlaybackState> m_playback_state = PlaybackState::Ready;
    std::atomic<std::chrono::milliseconds> m_position_ms = std::chrono::milliseconds{0};
};

#endif // MUSICPLAYER_H
