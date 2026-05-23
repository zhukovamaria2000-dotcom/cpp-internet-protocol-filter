#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

struct Session {
    int startSec, endSec;  // Время в секундах для простоты сравнения
    long long received, sent;
    string program;

    bool contains(const string& name) const {
        string p = program, n = name;
        transform(p.begin(), p.end(), p.begin(), ::tolower);
        transform(n.begin(), n.end(), n.begin(), ::tolower);
        return p.find(n) != string::npos;
    }

    string startTime() const {
        int h = startSec / 3600;
        int m = (startSec % 3600) / 60;
        int s = startSec % 60;
        char buf[9];
        sprintf(buf, "%02d:%02d:%02d", h, m, s);
        return buf;
    }

    string endTime() const {
        int h = endSec / 3600;
        int m = (endSec % 3600) / 60;
        int s = endSec % 60;
        char buf[9];
        sprintf(buf, "%02d:%02d:%02d", h, m, s);
        return buf;
    }
};

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

// Вывод таблицы
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

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "Лабораторная работа №8\n";
    cout << "Вариант: Протокол работы в Интернет\n";
    cout << "Студент: Мария Жукова\n";
    cout << "Номер группы: 25ИСиТ1д\n\n";

    ifstream file("protocol.txt");
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть protocol.txt\n";
        return 1;
    }

    vector<Session> allSessions;
    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;
        Session s;
        if (parseLine(line, s)) {
            allSessions.push_back(s);
        }
        else {
            cerr << "Ошибка в строке " << lineNum << ": " << line << "\n";
        }
    }
    file.close();

    if (allSessions.empty()) {
        cout << "Нет данных\n";
        return 0;
    }

    int choice;
    do {
        cout << "\n======================================\n";
        cout << "      ФИЛЬТРАЦИЯ ПРОТОКОЛА          \n";
        cout << "=====================================\n";
        cout << "  1. По программе                    \n";
        cout << "  2. После времени                   \n";
        cout << "  3. Диапазон времени                \n";
        cout << "  4. По полученным данным (>=)       \n";
        cout << "  5. По отправленным данным (>=)     \n";
        cout << "  6. Все записи                      \n";
        cout << "  0. Выход                           \n";
        cout << "======================================\n";
        cout << "Выбор: ";
        cin >> choice;
        cin.ignore();

        vector<Session> result;

        switch (choice) {
        case 1: {
            cout << "Название программы: ";
            string prog;
            getline(cin, prog);
            for (const auto& s : allSessions)
                if (s.contains(prog)) result.push_back(s);
            printSessions(result, "Программа: " + prog);
            break;
        }
        case 2: {
            cout << "Время (ЧЧ:ММ:СС): ";
            string t;
            getline(cin, t);
            int sec = toSeconds(t);
            for (const auto& s : allSessions)
                if (s.startSec >= sec) result.push_back(s);
            printSessions(result, "Сеансы после " + t);
            break;
        }
        case 3: {
            cout << "Начало (ЧЧ:ММ:СС): ";
            string t1, t2;
            getline(cin, t1);
            cout << "Конец (ЧЧ:ММ:СС): ";
            getline(cin, t2);
            int startSec = toSeconds(t1);
            int endSec = toSeconds(t2);
            for (const auto& s : allSessions)
                if (s.startSec >= startSec && s.startSec <= endSec)
                    result.push_back(s);
            printSessions(result, "Диапазон: " + t1 + " - " + t2);
            break;
        }
        case 4: {
            cout << "Минимум байт: ";
            long long minBytes;
            cin >> minBytes;
            for (const auto& s : allSessions)
                if (s.received >= minBytes) result.push_back(s);
            printSessions(result, "Получено >= " + to_string(minBytes) + " байт");
            break;
        }
        case 5: {
            cout << "Минимум байт: ";
            long long minBytes;
            cin >> minBytes;
            for (const auto& s : allSessions)
                if (s.sent >= minBytes) result.push_back(s);
            printSessions(result, "Отправлено >= " + to_string(minBytes) + " байт");
            break;
        }
        case 6:
            printSessions(allSessions, "ПОЛНЫЙ ПРОТОКОЛ");
            break;
        case 0:
            cout << "До свидания!\n";
            break;
        default:
            cout << "Неверный выбор!\n";
        }

        if (choice != 0 && choice != 6) {
            cout << "\nНажмите Enter для продолжения...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 0);

    return 0;
}
