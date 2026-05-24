#pragma once

#include <vector>
#include "Session.h"

using namespace std;

// Функции сравнения (возвращают: -1 если a < b, 0 если a == b, 1 если a > b)

// Сравнение по убыванию длительности сеанса (чем дольше, тем выше)
int cmpByDurationDesc(const Session* a, const Session* b);

/* Сравнение: сначала по названию программы(возрастание),
затем по суммарному трафику (убывание)
*/
int cmpByProgramAndTraffic(const Session* a, const Session* b);

void insertionSort(vector<Session*>& arr, int (*cmp)(const Session*, const Session*));

void quickSort(vector<Session*>& arr, int left, int right, int (*cmp)(const Session*, const Session*));

// Вспомогательная функция для вызова быстрой сортировки (удобная обёртка)
void quickSort(vector<Session*>& arr, int (*cmp)(const Session*, const Session*));