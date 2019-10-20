#ifndef IMEDIAPLAYER_H
#define IMEDIAPLAYER_H

#include <array>
#include <chrono>
#include <filesystem>
#include <mutex>
#include <string>

namespace fs = std::filesystem;

class Track;

class IMediaPlayer
{
public:
    enum class State {
        Stopped = 0,
        Playing,
        Paused,
        Ready
    };
    std::string displayState(State s) const;

    IMediaPlayer();

    void exec();

    static fs::path convertToAbsolutePath(const std::string& file);
    void play(const fs::path& file);
    void pause();
    void resume();
    void stop();
    void trackPosition() const;

    void playTrack(const Track& track);

    static constexpr std::array<const char*, 3> k_valid_track_ext = {".mp3d",".omg",".flaque"};
    static constexpr const char* k_valid_playlist_ext = ".plst";

private:
    static bool isTrackExtValid(const char* ext);

    std::chrono::milliseconds position() const;
    void setPosition(std::chrono::milliseconds trackPosition);

    State state() const;
    void setState(State state);

    void trackPlaybackSimu(std::chrono::milliseconds duration);

private:
    mutable std::mutex m_postion_lock;
    std::chrono::milliseconds m_position_ms = std::chrono::milliseconds{0};

    mutable std::mutex m_state_lock;
    State m_state = State::Ready;
};

#endif // IMEDIAPLAYER_H
