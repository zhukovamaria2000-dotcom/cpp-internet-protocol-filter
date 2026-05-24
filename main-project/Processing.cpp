#include "Processing.h"
#include <algorithm>
#include <cctype>

using namespace std;

long long getTotalDurationForProgram(const vector<Session>& sessions, const string& programName) {
    long long totalDuration = 0;

    string searchName = programName;
    transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

    for (const auto& session : sessions) {
        string progLower = session.program;
        transform(progLower.begin(), progLower.end(), progLower.begin(), ::tolower);

        // Если содержит искомое имя 
        if (progLower.find(searchName) != string::npos) {
            totalDuration += session.duration();
        }
    }

    return totalDuration;
}