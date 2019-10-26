#include <future>
#include <iostream>
#include <sstream>
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

    std::cout << "\nYou can use the following commands:"
              << "\n\tadd_track \"track_path\", play (\"track_path\")"
              << "\n\tpause, stop, next, previous, position"
              << "\n\tremove \"track_path\", remove_duplicates"
              << "\n\trepeat, random, exit"
              << "\n\tshow_track \"track_path\", show_playlist"
              << "\n\n" << std::endl;
}

static std::string readUserInput()
{
    std::string user_input;
    std::getline(std::cin, user_input);
    return user_input;
}

void IMediaPlayer::exec()
{
    std::cout << "IMediaPlayer starting.\n";
    std::future<std::string> user_input = std::async(std::launch::async, readUserInput);

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
            std::cout << "Which track would you like to add ?" << std::endl;
        }
    }
    else if (command == "remove")
    {
        if (args.size() > 1)
        {
            removeTrack(args.at(1));
        }
        else
        {
            std::cout << "Which track would you like to remove ?" << std::endl;
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
    else if (command == "remove_duplicates")
    {
        m_playlist.removeDuplicates();
    }
    else if (command == "repeat")
    {
        m_playlist.toggleRepeat();
    }
    else if (command == "random")
    {
        m_playlist.toggleRandom();
    }
    else if (command == "show_track")
    {
        if (args.size() > 1)
        {
            fs::path p = file_loader::convertToAbsolutePath(args.at(1));
            showTrack(p);
        }
        else
        {
            showTrack(m_playlist.currentTrack());
        }
    }
    else if (command == "show_playlist")
    {
        std::cout << "\n" << m_playlist.displayInfo() << std::endl;
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
    if (m_music_player.playbackState() == MusicPlayer::PlaybackState::Ended
            && !m_playlist.isFinished())
    {
        m_playlist.next();
        readCurrentTrack();
    }
}

void IMediaPlayer::readCurrentTrack()
{        
    if (m_playlist.isFinished())
    {
        std::cout << "\tPlaylist finished\n" << std::endl;
        return;
    }

    fs::path current_track = m_playlist.currentTrack();

    if (auto optional_track = loadTrack(current_track))
    {
        const Track& track = optional_track.value();
        std::cout << "\n" << m_playlist.displayInfo() << "\n"
                  << track.displayInfo() << "\n" << std::endl;

        m_music_player.playTrack(track);
    }
    else // Invalid track
    {
        m_playlist.next();
        m_playlist.remove(current_track);
        std::cout << "Invalid track removed" << std::endl;
    }
}

std::optional<Track> IMediaPlayer::loadTrack(const fs::path& file_absolut_path)
{
    // Check if file hasn't been removed
    if (file_absolut_path.empty() || !file_loader::doesFileExist(file_absolut_path))
        return {};

    return file_loader::parseTrack(file_absolut_path);
}

bool IMediaPlayer::isTrackValid(const fs::path& p)
{
    std::string ext = p.extension().string();

    if (file_loader::doesFileExist(p))
    {
        if(MusicPlayer::isTrackExtValid(ext.c_str()))
        {
            return true;
        }
        else
        {
            std::cout << "The " << ext << " files, can't be read" << std::endl;
        }
    }

    return false;
}

void IMediaPlayer::addTrack(const std::string& file)
{
    fs::path p = file_loader::convertToAbsolutePath(file);

    if (isTrackValid(p))
    {
        std::cout << "\tAdded " << p.filename() << std::endl;
        m_playlist.add(p);
    }
}

void IMediaPlayer::removeTrack(const std::string& file)
{
    fs::path p = file_loader::convertToAbsolutePath(file);
    m_playlist.remove(p);
}

void IMediaPlayer::showTrack(const fs::path& p)
{
    if (isTrackValid(p))
    {
        if (auto optional_track = loadTrack(p))
        {
            std::cout << "\n" << optional_track.value().displayInfo() << std::endl;
            return;
        }
    }

    std::cout << "Invalid track" << std::endl;
}

void IMediaPlayer::play()
{
    if (m_music_player.playbackState() == MusicPlayer::PlaybackState::Paused)
    {
        m_music_player.request(MusicPlayer::Command::Play);
    }
    else
    {
        if (m_music_player.isPlaybackOver())
        {
            m_playlist.play();
            readCurrentTrack();
        }
    }
}

void IMediaPlayer::next()
{
    if (!m_music_player.isPlaybackOver())
        m_music_player.request(MusicPlayer::Command::Stop);
    m_playlist.next();
    readCurrentTrack();
}

void IMediaPlayer::previous()
{
    if (!m_music_player.isPlaybackOver())
        m_music_player.request(MusicPlayer::Command::Stop);
    m_playlist.previous();
    readCurrentTrack();
}

void IMediaPlayer::pause()
{
    if (!m_music_player.isPlaybackOver())
        m_music_player.request(MusicPlayer::Command::Pause);
}

void IMediaPlayer::stop()
{
    if (!m_music_player.isPlaybackOver())
        m_music_player.request(MusicPlayer::Command::Stop);
    else
        std::cout << "\tTrack already stopped\n" << std::endl;

    m_playlist.stop();
}

void IMediaPlayer::trackPosition() const
{
    std::cout << "\tTrack position: " << utils::displayDuration(m_music_player.position()) << "\n" << std::endl;
}
