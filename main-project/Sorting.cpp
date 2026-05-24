#include "Sorting.h"
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

int cmpByDurationDesc(const Session* a, const Session* b) {
    int durA = a->duration();
    int durB = b->duration();

    if (durA > durB) return -1;  // a длиннее, то a выше
    if (durA < durB) return 1;   // b длиннее, то b выше
    return 0;                     // равны
}

// 2. Сравнение: сначала по названию программы (A-Z), затем по трафику (от большего к меньшему)
int cmpByProgramAndTraffic(const Session* a, const Session* b) {
    string progA = a->program;
    string progB = b->program;

    transform(progA.begin(), progA.end(), progA.begin(), ::tolower);
    transform(progB.begin(), progB.end(), progB.begin(), ::tolower);

    // Сравниваем строки
    if (progA < progB) return -1;  // A раньше по алфавиту
    if (progA > progB) return 1;   // B раньше по алфавиту

    // Если программы одинаковые, сравниваем по суммарному трафику (убывание)
    long long trafficA = a->totalTraffic();
    long long trafficB = b->totalTraffic();

    if (trafficA > trafficB) return -1;
    if (trafficA < trafficB) return 1;
    return 0;  // равны
}

// Принцип: берём элемент и вставляем его в правильную позицию в отсортированной части
void insertionSort(vector<Session*>& arr, int (*cmp)(const Session*, const Session*)) {
    int n = (int)arr.size();

    for (int i = 1; i < n; i++) {
        Session* current = arr[i];
        int j = i - 1;

        while (j >= 0 && cmp(arr[j], current) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = current;
    }
}

// Разделение массива: все элементы меньше опорного — слева, больше — справа
static int partition(vector<Session*>& arr, int left, int right, int (*cmp)(const Session*, const Session*)) {
    Session* pivot = arr[right];  // опорный элемент
    int i = left - 1;             // индекс для меньших элементов

    for (int j = left; j < right; j++) {
        if (cmp(arr[j], pivot) <= 0) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    // Ставим опорный элемент на правильное место
    swap(arr[i + 1], arr[right]);
    return i + 1;  // возвращаем индекс опорного элемента
}

void quickSort(vector<Session*>& arr, int left, int right, int (*cmp)(const Session*, const Session*)) {
    if (left < right) {
        int pi = partition(arr, left, right, cmp);

        quickSort(arr, left, pi - 1, cmp);   // сортируем левую часть
        quickSort(arr, pi + 1, right, cmp);  // сортируем правую часть
    }
}

// Обёртка для удобного вызова быстрой сортировки
void quickSort(vector<Session*>& arr, int (*cmp)(const Session*, const Session*)) {
    if (!arr.empty()) {
        quickSort(arr, 0, (int)arr.size() - 1, cmp);
    }
}