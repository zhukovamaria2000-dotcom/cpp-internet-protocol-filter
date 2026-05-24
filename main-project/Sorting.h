#pragma once

#include <vector>
#include "Session.h" 

using namespace std;

int cmpByDurationDesc(const Session* a, const Session* b);
int cmpByProgramAndTraffic(const Session* a, const Session* b);

void insertionSort(vector<Session*>& arr, int (*cmp)(const Session*, const Session*));
void quickSort(vector<Session*>& arr, int left, int right, int (*cmp)(const Session*, const Session*));
void quickSort(vector<Session*>& arr, int (*cmp)(const Session*, const Session*));