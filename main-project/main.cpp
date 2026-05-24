#include <iostream>
#include <locale>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>

#include "Session.h"
#include "FileReader.h"
#include "constants.h"
#include "TablePrinter.h"
#include "Filters.h"
#include "Sorting.h"
#include "Processing.h"

using namespace std;

vector<Session*> createPointerArray(const vector<Session>& sessions) {
    vector<Session*> pointers;
    for (const auto& s : sessions) {
        pointers.push_back(const_cast<Session*>(&s));
    }
    return pointers;
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "Лабораторная работа №8\n";
    cout << "Вариант: Протокол работы в Интернет\n";
    cout << "Студент: Мария Жукова\n";
    cout << "Номер группы: 25ИСиТ1д (очная форма)\n"; // - изменено в ветке main
    cout << "Факультет:ФМиИТ\n\n";

    vector<Session> allSessions = readSessionsFromFile(DATA_FILE);

    if (allSessions.empty()) {
        cout << "Нет данных\n";
        return 0;
    }

    int choice;
    do {
        cout << "\n=================================================\n";
        cout << "            ФИЛЬТРАЦИЯ ПРОТОКОЛА                \n";
        cout << "=================================================\n";
        cout << "  1. Программа Skype                             \n";
        cout << "  2. После времени 08:00:00                      \n";
        cout << "  3. Диапазон времени                            \n";
        cout << "  4. По полученным данным (>=)                   \n";
        cout << "  5. По отправленным данным (>=)                 \n";
        cout << "  6. Все записи                                  \n";
        cout << "  7. Сортировка                                  \n";
        cout << "  8. Суммарное время использования сети Интернет \n";
        cout << "  0. Выход                                       \n";
        cout << "=================================================\n";
        cout << "Выбор: ";
        cin >> choice;
        cin.ignore();

        vector<Session> result;

        switch (choice) {
        case 1: {
            result = filterSessions(allSessions, filterBySkype);
            printSessions(result, "Протокол программы Skype ");
            break;
        }
        case 2: {
            result = filterSessions(allSessions, filterAfter8AM);
            printSessions(result, "Сеансы после 08:00:00");
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
        case 7: {  
            cout << "\n--- Сортировка  данных ---\n";
            cout << "Выберите метод сортировки:\n";
            cout << "  1. Сортировка вставками\n";
            cout << "  2. Быстрая сортировка\n";
            cout << "Выбор: ";
            int sortMethod;
            cin >> sortMethod;
            cin.ignore();

            cout << "\nВыберите критерий сортировки:\n";
            cout << "  1. По убыванию длительности сеанса\n";
            cout << "  2. По возрастанию программы, затем по убыванию трафика\n";
            cout << "Выбор: ";
            int sortCriterion;
            cin >> sortCriterion;
            cin.ignore();

            // Создаём массив указателей 
            vector<Session*> pointers = createPointerArray(allSessions);

            int (*cmp)(const Session*, const Session*) = nullptr;
            if (sortCriterion == 1) {
                cmp = cmpByDurationDesc;
                cout << "\nКритерий: Убывание длительности сеанса\n";
            }
            else if (sortCriterion == 2) {
                cmp = cmpByProgramAndTraffic;
                cout << "\nКритерий: Программа (A-Z) -> Трафик (убывание)\n";
            }
            else {
                cout << "Неверный выбор критерия!\n";
                break;
            }

            if (sortMethod == 1) {
                cout << "Метод: Сортировка вставками\n";
                insertionSort(pointers, cmp);
            }
            else if (sortMethod == 2) {
                cout << "Метод: Быстрая сортировка\n";
                quickSort(pointers, cmp);
            }
            else {
                cout << "Неверный выбор метода!\n";
                break;
            }

            cout << "\n--- ОТСОРТИРОВАННЫЙ ПРОТОКОЛ ---\n";
            cout << left << setw(12) << "НАЧАЛО" << setw(12) << "КОНЕЦ"
                << setw(15) << "ПОЛУЧЕНО" << setw(15) << "ОТПРАВЛЕНО"
                << setw(20) << "ДЛИТЕЛЬНОСТЬ" << "ПРОГРАММА\n";
            cout << string(90, '-') << "\n";

            for (const auto* s : pointers) {
                cout << left << setw(12) << s->startTime()
                    << setw(12) << s->endTime()
                    << setw(15) << s->received
                    << setw(15) << s->sent
                    << setw(20) << s->duration()
                    << s->program << "\n";
            }
            break;
        }
        case 8: {
            cout << "Введите название программы: ";
            string progName;
            getline(cin, progName);

            long long totalTime = getTotalDurationForProgram(allSessions, progName);

            cout << "\nСуммарное время использования программы \"" << progName << "\": ";

            int hours = totalTime / 3600;
            int minutes = (totalTime % 3600) / 60;
            int seconds = totalTime % 60;

            cout << hours << "ч " << minutes << "м " << seconds << "с\n";
            break;
        }
        case 0:
            cout << "До свидания!\n";
            break;
        default:
            cout << "Неверный выбор!\n";
        }

        if (choice != 0 && choice != 8) {
            cout << "\nНажмите Enter для продолжения...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 0);

    return 0;
}