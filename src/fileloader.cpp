#include <iostream>
#include <fstream>

#include "fileloader.h"


fs::path file_loader::convertToAbsolutePath(const std::string& file)
{
    fs::path path{file};

    if (path.is_relative())
    {
        return fs::absolute(path);
    }

    return path;
}

bool file_loader::doesFileExist(const fs::path& path)
{
    std::error_code ec;

    if (fs::exists(path, ec))
    {
        return true;
    }
    else
    {
        std::cout << "File not found: " << path << std::endl;

        if (ec)
            std::cout << " (" << ec.message() << ")" << std::endl;
    }

    return false;
}

std::optional<Track> file_loader::parseTrack(const fs::path& file_path)
{
    std::ifstream track_stream(file_path.string().c_str());

    if (track_stream.is_open())
    {
        Track track{file_path};

        if (track_stream >> track)
        {
            track_stream.close();
            return std::optional<Track>{track};
        }
        else
        {
            std::cout << "Failed to load track" << std::endl;
        }

        track_stream.close();
    }

    return {};
}
