#pragma once
#include <string>
using namespace std;

struct Session {
    int startSec, endSec;
    long long received, sent;
    string program;

    int duration() const { return endSec - startSec; }
    long long totalTraffic() const { return received + sent; }
};