#pragma once

#include <string>
#include <vector>
#include "Session.h"

using namespace std;

bool filterBySkype(const Session& s);

bool filterAfter8AM(const Session& s);

// Универсальная функция фильтрации: принимает ВЕКТОР и УКАЗАТЕЛЬ НА ФУНКЦИЮ-ФИЛЬТР
vector<Session> filterSessions(const vector<Session>& src, bool (*predicate)(const Session&));