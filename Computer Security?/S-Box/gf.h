#ifndef WORK_CODE_GF_H
#define WORK_CODE_GF_H

#define M 8

#include <vector>

using namespace std;

void transform(unsigned k, int vec[], int n);

unsigned gf28_mul(unsigned a1, unsigned a2);

unsigned marge(vector<int> &arr);

#endif //WORK_CODE_GF_H
