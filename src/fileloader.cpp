#include <iostream>
#include <fstream>

#include "fileloader.h"

std::optional<Track> FileLoader::loadTrack(const fs::path& file_path)
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
