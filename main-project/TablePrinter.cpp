#include "TablePrinter.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void printSessions(const vector<Session>& sessions, const string& title) {
    cout << "\n========================================\n";
    cout << title << "\n";
    cout << "========================================\n";
    cout << left << setw(12) << "НАЧАЛО" << setw(12) << "КОНЕЦ"
        << setw(15) << "ПОЛУЧЕНО" << setw(15) << "ОТПРАВЛЕНО"
        << "ПРОГРАММА\n";
    cout << string(80, '-') << "\n";

    for (const auto& s : sessions) {
        cout << left << setw(12) << s.startTime() << setw(12) << s.endTime()
            << setw(15) << s.received << setw(15) << s.sent
            << s.program << "\n";
    }

    if (sessions.empty()) cout << "Нет данных\n";
}