#include <iostream> 
#include <sstream>
#include <fstream>
#include <cctype>
#include "FileReader.h" 

using namespace std;


// Преобразует "чч:мм:сс" в секунды
int toSeconds(const string& time) {
    int h, m, s;
    char c1, c2;
    istringstream(time) >> h >> c1 >> m >> c2 >> s;
    return h * 3600 + m * 60 + s;
}

// Разбор строки файла
bool parseLine(const string& line, Session& s) {
    istringstream iss(line);
    string startStr, endStr;

    if (!(iss >> startStr >> endStr >> s.received >> s.sent)) return false;

    s.startSec = toSeconds(startStr);
    s.endSec = toSeconds(endStr);

    getline(iss, s.program);
    size_t pos = s.program.find_first_not_of(" \t");
    if (pos != string::npos) s.program = s.program.substr(pos);

    return true;
}

vector<Session> readSessionsFromFile(const string& filename) {
    vector<Session> sessions;  
    ifstream file(filename);   

    
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return sessions;  
    }

    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;
        Session s;

        if (parseLine(line, s)) {
            sessions.push_back(s);  
        }
        else {
            cerr << "Ошибка в строке " << lineNum << ": " << line << endl;
        }
    }

    file.close();  
    return sessions;  
}
