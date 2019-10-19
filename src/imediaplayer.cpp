#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "imediaplayer.h"


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
    if (fs::exists(file))
    {
        std::string ext = file.extension().string();

        if (isTrackExtValid(ext.c_str()))
        {
            std::cout << "Track to play " << file << std::endl;
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
    }
}

void IMediaPlayer::resume()
{
    std::cout << "Resume" << std::endl;
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
