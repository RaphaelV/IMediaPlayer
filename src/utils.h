#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <iterator>
#include <random>
#include <sstream>
#include <string>

namespace utils {

template<typename Iter>
Iter getRandomElement(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(gen));
    return start;
}

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
        ss << hour.count() << "h";

    ss << mins.count() << "m ";

    ss << secs.count() << "s";

    return ss.str();
}

}

#endif // UTILS_H
