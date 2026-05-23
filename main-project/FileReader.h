#pragma once

#include <vector>
#include <string>
#include "Session.h"

using namespace std;

int toSeconds(const string& time);
vector<Session> readSessionsFromFile(const string& filename);