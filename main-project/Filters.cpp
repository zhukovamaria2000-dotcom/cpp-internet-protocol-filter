#include "Filters.h"
#include <algorithm>  
#include <cctype>     

using namespace std;


bool filterBySkype(const Session& s) {
    // Копируем название программы в нижний регистр
    string progLower = s.program;
    transform(progLower.begin(), progLower.end(), progLower.begin(), ::tolower);

    return progLower.find("skype") != string::npos;
}

bool filterAfter8AM(const Session& s) {
    const int EIGHT_AM_SECONDS = 8 * 3600;  
    return s.startSec >= EIGHT_AM_SECONDS;
}


// Принимает: вектор сеансов и указатель на функцию-фильтр
// Возвращает: новый вектор с элементами, для которых фильтр вернул true
vector<Session> filterSessions(const vector<Session>& src, bool (*predicate)(const Session&)) {
    vector<Session> result;

    for (const auto& session : src) {
        if (predicate(session)) {  
            result.push_back(session);
        }
    }

    return result;
}