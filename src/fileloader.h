#ifndef FILELOADER_H
#define FILELOADER_H

#include <optional>

#include "track.h"

struct FileLoader
{
    static std::optional<Track> loadTrack(const fs::path& file_path);
};

#endif // FILELOADER_H
