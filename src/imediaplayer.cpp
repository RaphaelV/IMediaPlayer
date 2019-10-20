#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>

#include "fileloader.h"
#include "track.h"
#include "utils.h"

#include "imediaplayer.h"


std::string IMediaPlayer::displayState(State s) const
{
    if (s == State::Playing)
        return "Playing";

    if (s == State::Paused)
        return "Paused";

    if (s == State::Stopped)
        return "Stopped";

    return "";
}

IMediaPlayer::IMediaPlayer()
{
    std::cout << "IMediaPlayer reads the following music file formats:";

    for (const char* ext : k_valid_track_ext)
    {
        std::cout << " " << ext ;
    }

    std::cout << std::endl;
    std::cout << "You can also open playlists (" << k_valid_playlist_ext << ")" << std::endl;
}

void IMediaPlayer::exec()
{
    std::cout << "IMediaPlayer starting." << std::endl;

    std::string user_input, arg;

    while (std::getline(std::cin, user_input))
    {
        std::istringstream iss(user_input);
        std::vector<std::string> args;

        while(iss >> arg)
        {
            args.push_back(arg);
        }

        if (args.empty())
            continue;

        const auto& command = args.front();

        if (command == "play")
        {
            if (args.size() > 1)
            {
                fs::path file = convertToAbsolutePath(args.at(1));
                play(file);
            }
            else
            {
                resume();
            }
        }
        else if (command == "pause")
        {
            pause();
        }
        else if (command == "stop")
        {
            stop();
        }
        else if (command == "pos")
        {
            trackPosition();
        }
        else if (command == "exit")
        {
            std::cout << "IMediaPlayer stoping." << std::endl;
            break;
        }
        else
        {
            std::cout << "Unknown command." << std::endl;
        }
    }
}

fs::path IMediaPlayer::convertToAbsolutePath(const std::string& file)
{
    fs::path path{file};

    if (path.is_relative())
    {
        return fs::absolute(path);
    }

    return path;
}

void IMediaPlayer::play(const fs::path& file)
{
    std::error_code ec;

    if (fs::exists(file, ec))
    {
        std::string ext = file.extension().string();

        if (isTrackExtValid(ext.c_str()))
        {
            if (auto track_to_play = FileLoader::loadTrack(file.string()))
            {
                playTrack(track_to_play.value());
            }
        }
        else if (ext == k_valid_playlist_ext)
        {
            std::cout << "Playlist to play " << file << std::endl;
        }
        else
        {
            std::cout << "The " << ext << " files, can't be read" << std::endl;
        }
    }
    else
    {
        std::cout << "File not found: " << file << std::endl;

        if (ec)
            std::cout << " (" << ec.message() << ")" << std::endl;
    }
}

void IMediaPlayer::pause()
{
    if (state() == State::Playing)
        setState(State::Paused);
}

void IMediaPlayer::resume()
{
    if (state() == State::Paused)
        setState(State::Playing);
}

void IMediaPlayer::stop()
{
    setState(State::Stopped);
}

void IMediaPlayer::trackPosition() const
{
    std::cout << "\tTrack position: " << utils::displayDuration(position()) << std::endl;
}

void IMediaPlayer::playTrack(const Track& track)
{
    auto s = state();

    if (s == State::Ready)
    {
        std::cout << "\n" << "\n" << track.displayInfo() << std::endl;
        auto d = track.duration();

        std::thread t{[this, d]() { trackPlaybackSimu(d); }};
        t.detach();
    }
    else
    {
        std::cout << "Track already playing" << std::endl;
    }
}

bool IMediaPlayer::isTrackExtValid(const char* ext)
{
    auto it = std::find_if(k_valid_track_ext.begin(), k_valid_track_ext.end(),
                           [ext](const char* valid_ext)
    {
        return std::strcmp(ext, valid_ext);
    });

    return it != k_valid_track_ext.end();
}

std::chrono::milliseconds IMediaPlayer::position() const
{
    std::lock_guard<std::mutex> lk(m_postion_lock);
    return m_position_ms;
}

void IMediaPlayer::setPosition(std::chrono::milliseconds pos)
{
    std::lock_guard<std::mutex> lk(m_postion_lock);
    m_position_ms = pos;
}

IMediaPlayer::State IMediaPlayer::state() const
{
    std::lock_guard<std::mutex> lk(m_state_lock);
    return  m_state;
}

void IMediaPlayer::setState(IMediaPlayer::State state)
{
    std::lock_guard<std::mutex> lk(m_state_lock);
    m_state = state;
}

void IMediaPlayer::trackPlaybackSimu(std::chrono::milliseconds duration)
{
    setPosition(std::chrono::milliseconds{0});
    setState(State::Playing);

    auto update_state = [this](State& local_state)
    {
        if (local_state != state())
        {
            local_state = state();
            std::cout << "\t" << displayState(local_state) << ", pos: " << utils::displayDuration(position()) << std::endl;
        }
    };

    State local_state = State::Stopped;
    update_state(local_state);

    for (;local_state != State::Stopped; update_state(local_state))
    {
        if (local_state == State::Playing)
        {
            auto one_sec = std::chrono::seconds{1};
            // Sound playing simulation
            std::this_thread::sleep_for(one_sec);
            auto p = position() + one_sec;

            if (p > duration)
                break;

            setPosition(p);
            //            std::cout << "\rPosition: " << utils::displayDuration(p) << "/" << utils::displayDuration(duration) << "\t";
        }
        else
        {
            // Wait before checking state again
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
        }
    }

    std::cout << "\tEnd track" << std::endl;
    setState(State::Ready);
}
