#pragma once  

#include <string>  

using namespace std; 


struct Session {
   
    int startSec;      
    int endSec;        
    long long received; 
    long long sent;     
    string program;     

    // Проверка, содержит ли программа указанную подстроку
    bool contains(const string& name) const {
        string p = program, n = name;
        transform(p.begin(), p.end(), p.begin(), ::tolower);
        transform(n.begin(), n.end(), n.begin(), ::tolower);
        return p.find(n) != string::npos;
    }

    // Возвращает время начала в формате "ЧЧ:ММ:СС"
    string startTime() const {
        int h = startSec / 3600;
        int m = (startSec % 3600) / 60;
        int s = startSec % 60;
        char buf[9];
        sprintf(buf, "%02d:%02d:%02d", h, m, s);
        return buf;
    }

    // Возвращает время окончания в формате "ЧЧ:ММ:СС"
    string endTime() const {
        int h = endSec / 3600;
        int m = (endSec % 3600) / 60;
        int s = endSec % 60;
        char buf[9];
        sprintf(buf, "%02d:%02d:%02d", h, m, s);
        return buf;
    }

    // Возвращает длительность сеанса в секундах
    int duration() const {
        return endSec - startSec;
    }

    // Возвращает суммарный трафик (получено + отправлено)
    long long totalTraffic() const {
        return received + sent;
    }
};