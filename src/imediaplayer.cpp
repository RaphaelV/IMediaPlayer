#include <iostream>
#include <sstream>
#include <vector>

#include "imediaplayer.h"

IMediaPlayer::IMediaPlayer()
{
    std::cout << "IMediaPlayer reads the following music file formats:";

    for (const char* ext : k_valid_track_ext)
    {
        std::cout << " ." << ext ;
    }

    std::cout << std::endl;
    std::cout << "You can also open playlists (." << k_valid_playlist_ext << ")" << std::endl;
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
            std::cout << "Command: " << command << " " << args.size() << std::endl;
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
