#include <iostream>
#include "imediaplayer.h"

using namespace std;

int main()
{
    // Cin will by default flush cout, we don't want this feature with mutiple threads
    std::cin.tie(nullptr);
    IMediaPlayer mp;
    mp.exec();

    return 0;
}
