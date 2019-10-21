#ifndef FILELOADER_H
#define FILELOADER_H

#include <optional>

#include "track.h"

namespace file_loader
{

fs::path convertToAbsolutePath(const std::string& file);

bool doesFileExist(const fs::path& path);

std::optional<Track> parseTrack(const fs::path& file_path);

};


#endif // FILELOADER_H
