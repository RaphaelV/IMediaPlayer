#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <sstream>
#include <string>

namespace utils {

inline std::string displayDuration(std::chrono::milliseconds ms)
{
    using namespace std::chrono;
    auto secs = duration_cast<seconds>(ms);
    ms -= duration_cast<milliseconds>(secs);
    auto mins = duration_cast<minutes>(secs);
    secs -= duration_cast<seconds>(mins);
    auto hour = duration_cast<hours>(mins);
    mins -= duration_cast<minutes>(hour);

    std::stringstream ss;

    if (hour.count())
        ss << hour.count() << "h ";

    if (mins.count())
        ss << mins.count() << "m ";

    ss << secs.count() << "s";

    return ss.str();
}

}

#endif // UTILS_H
