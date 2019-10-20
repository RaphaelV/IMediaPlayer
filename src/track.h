#ifndef TRACK_H
#define TRACK_H

#include <chrono>
#include <filesystem>
#include <sstream>
#include <string>


namespace fs = std::filesystem;

struct MetaData
{
    std::string title = "";
    std::string artist = "";
    std::string album = "";
    std::chrono::milliseconds duration_ms = std::chrono::milliseconds{0};
    std::string codec = "";

    inline bool isValid() const;

    friend std::ostream& operator << (std::ostream& out, const MetaData& meta_data);
    friend std::istream& operator >> (std::istream& in,  MetaData& meta_data);
};

bool MetaData::isValid() const
{
    // It is considered that duration is mandatory
    return duration_ms.count() != 0;
}

std::ostream& operator << (std::ostream& out, const MetaData& meta_data);
std::istream& operator >> (std::istream& in,  MetaData& meta_data);

class Track
{
public:
    Track(const fs::path& file_path);

    bool isValid() const;

    std::string displayInfo() const;

    std::chrono::milliseconds duration() const;

private:
    friend std::ostream& operator << (std::ostream& out, const Track& track);
    friend std::istream& operator >> (std::istream& in,  Track& track);

private:

    fs::path m_file_path;
    MetaData m_metadata;
};

inline std::ostream& operator << (std::ostream& out, const Track& track)
{
    out << "Track file: " << track.m_file_path << std::endl;
    out <<  track.m_metadata << std::endl;
    return out;
}

inline std::istream& operator >> (std::istream& in, Track& track)
{
    in >> track.m_metadata;
    return in;
}

#endif // TRACK_H
