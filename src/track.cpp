#include <iostream>

#include "utils.h"

#include "track.h"

std::ostream& operator << (std::ostream& out, const MetaData& meta_data)
{
    out <<  meta_data.title;
    out << ";" << meta_data.artist;
    out << ";" << meta_data.album;
    out << ";" << meta_data.codec;
    out << ";" << meta_data.duration_ms.count();

    return out;
}

std::istream& operator >> (std::istream& in,  MetaData& meta_data)
{
    std::getline(in, meta_data.title, ';');
    std::getline(in, meta_data.artist, ';');
    std::getline(in, meta_data.album, ';');
    std::getline(in, meta_data.codec, ';');

    std::string d;

    if (std::getline(in, d, ';'))
    {
        long long duration;
        std::istringstream(d) >> duration;
        meta_data.duration_ms = std::chrono::milliseconds(duration);
    }

    return in;
}

Track::Track(const fs::path& file_path) :
    m_file_path{file_path}
{

}

bool Track::isValid() const
{
    std::error_code ec;
    return fs::exists(m_file_path, ec) && !ec && m_metadata.isValid();
}

std::string Track::displayInfo() const
{
    std::stringstream ss;
    ss << "Track: " << m_metadata.artist << " - " << m_metadata.title << " ("<< m_metadata.album << ")\n"
       << "Duration: " << utils::displayDuration(m_metadata.duration_ms) << "\n"
       << "Codec: " << m_metadata.codec;

    return ss.str();
}

std::chrono::milliseconds Track::duration() const
{
    return m_metadata.duration_ms;
}
