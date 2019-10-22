#include <algorithm>
#include <future>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "fileloader.h"
#include "track.h"
#include "utils.h"

#include "imediaplayer.h"


IMediaPlayer::IMediaPlayer()
{
    std::cout << "IMediaPlayer reads the following music file formats:";

    for (const char* ext : MusicPlayer::k_valid_track_ext)
    {
        std::cout << " " << ext ;
    }

    std::cout << std::endl;
}

static std::string readUserInput()
{
    std::string user_input;
    std::getline(std::cin, user_input);
    return user_input;
}

void IMediaPlayer::exec()
{
    std::cout << "IMediaPlayer starting." << std::endl;

    std::future<std::string> user_input = std::async(std::launch::async, readUserInput);

    while (m_run)
    {
        if (user_input.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
        {
            parseCommand(user_input.get());
            user_input = std::async(std::launch::async, readUserInput);
        }

        readPlaylist();
        wait();
    }

    std::cout << "IMediaPlayer stopping." << std::endl;
}

void IMediaPlayer::exit()
{
    m_run = false;
}

void IMediaPlayer::parseCommand(const std::string& user_input)
{
    std::istringstream iss(user_input);
    std::vector<std::string> args;

    std::string arg;

    while(iss >> arg)
    {
        args.push_back(arg);
    }

    if (args.empty())
        return;

    const auto& command = args.front();

    if (command == "play")
    {
        if (args.size() > 1)
        {
            m_playlist.clear();
            addTrack(args.at(1));
            play();
        }
        else
        {
            play();
        }
    }
    else if (command == "add_track")
    {
        if (args.size() > 1)
        {
            addTrack(args.at(1));
        }
        else
        {
            std::cout << "What track would you like to add ?" << std::endl;
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
        exit();
    }
    else
    {
        std::cout << "Unknown command." << std::endl;
    }
}

void IMediaPlayer::readPlaylist()
{
    if (m_music_player.playbackState() == MusicPlayer::PlaybackState::Stopped)
    {
        if (m_playlist.hasNext())
        {
            if (auto optional_track = loadTrack(m_playlist.currentTrack()))
            {
                const auto& track = optional_track.value();
                m_music_player.playTrack(track);
            }
            else
            {
                if (!m_playlist.currentTrack().empty())
                    removeTrack(m_playlist.currentTrack());

                next();
            }
        }
        else
        {
            stop();
        }
    }
}

std::optional<Track> IMediaPlayer::loadTrack(const fs::path& file_absolut_path)
{
    // Check if file hasn't been removed
    if (file_absolut_path.empty() || !file_loader::doesFileExist(file_absolut_path))
        return {};

    return file_loader::parseTrack(file_absolut_path);
}

void IMediaPlayer::wait()
{
    std::this_thread::sleep_for(std::chrono::milliseconds{200});
}

void IMediaPlayer::addTrack(const std::string& file)
{
    fs::path p = file_loader::convertToAbsolutePath(file);
    std::string ext = p.extension().string();

    if (file_loader::doesFileExist(p))
    {
        if(MusicPlayer::isTrackExtValid(ext.c_str()))
        {
            m_playlist.add(p);
        }
        else
        {
            std::cout << "The " << ext << " files, can't be read" << std::endl;
        }
    }
}

void IMediaPlayer::removeTrack(const std::string& file)
{

}

void IMediaPlayer::removeTrack(const fs::path &file)
{

}

void IMediaPlayer::play()
{
    m_music_player.request(MusicPlayer::Command::Play);
}

void IMediaPlayer::next()
{

}

void IMediaPlayer::previous()
{

}

void IMediaPlayer::pause()
{
    m_music_player.request(MusicPlayer::Command::Pause);
}

void IMediaPlayer::stop()
{
    m_music_player.request(MusicPlayer::Command::Stop);
}

void IMediaPlayer::trackPosition() const
{
    std::cout << "\tTrack position: " << utils::displayDuration(m_music_player.position()) << std::endl;
}
