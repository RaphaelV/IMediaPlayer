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
    std::cout << "Welcome to IMediaPlay, an imaginary music player\n";
    std::cout << "\nIMediaPlayer can play the following music file formats:";

    for (const char* ext : MusicPlayer::k_valid_track_ext)
    {
        std::cout << " " << ext ;
    }

    std::cout << "\nYou can use the following commands:";
    std::cout << "\n\tadd_track <track_name/track_path>";
    std::cout << "\n\tplay optionnal(<track_name/track_path>)";
    std::cout << "\n\tpause, stop, next, previous (prev), positions (pos), exit";
    std::cout << "\n\n" << std::endl;
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

    addTrack("track.bad");
    addTrack("BAD");
    addTrack("track.omg");
    addTrack("track.bad.omg");
    addTrack("track.flaque");
    addTrack("track.mp3d");

    while (m_run)
    {
        if (user_input.wait_for(std::chrono::seconds(1)) == std::future_status::ready)
        {
            parseCommand(user_input.get());
            user_input = std::async(std::launch::async, readUserInput);
        }

        readPlaylist();
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
            stop();
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
    else if (command == "pos"
             || command == "position")
    {
        trackPosition();
    }
    else if (command == "next")
    {
        next();
    }
    else if (command == "prev"
             || command == "previous")
    {
        previous();
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
    if (m_music_player.playbackState() == MusicPlayer::PlaybackState::Stopped
            && !m_playlist.isFinished())
    {
        if (auto optional_track = loadTrack(m_playlist.currentTrack()))
        {
            m_music_player.playTrack(optional_track.value());
        }
        else
        {
            if (!m_playlist.currentTrack().empty())
                removeTrack(m_playlist.currentTrack());
        }

        m_playlist.next();
    }
}

std::optional<Track> IMediaPlayer::loadTrack(const fs::path& file_absolut_path)
{
    // Check if file hasn't been removed
    if (file_absolut_path.empty() || !file_loader::doesFileExist(file_absolut_path))
        return {};

    return file_loader::parseTrack(file_absolut_path);
}

void IMediaPlayer::addTrack(const std::string& file)
{
    fs::path p = file_loader::convertToAbsolutePath(file);
    std::string ext = p.extension().string();

    if (file_loader::doesFileExist(p))
    {
        if(MusicPlayer::isTrackExtValid(ext.c_str()))
        {
            std::cout << "\tAdded " << p.filename() << std::endl;
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
    if (m_music_player.playbackState() == MusicPlayer::PlaybackState::Paused)
    {
        m_music_player.request(MusicPlayer::Command::Play);
    }
    else
    {
        m_playlist.play();
    }
}

void IMediaPlayer::next()
{
    m_music_player.request(MusicPlayer::Command::Stop);
    m_playlist.next();
}

void IMediaPlayer::previous()
{
    m_music_player.request(MusicPlayer::Command::Stop);
    m_playlist.previous();
}

void IMediaPlayer::pause()
{
    m_music_player.request(MusicPlayer::Command::Pause);
}

void IMediaPlayer::stop()
{
    m_playlist.stop();
    m_music_player.request(MusicPlayer::Command::Stop);
}

void IMediaPlayer::trackPosition() const
{
    std::cout << "\tTrack position: " << utils::displayDuration(m_music_player.position()) << std::endl;
}
