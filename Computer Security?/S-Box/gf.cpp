#include "gf.h"

const int poly[M + 1] = {1, 1, 0, 1, 1, 0, 0, 0, 1};

//实现多项式系数转换
void transform(unsigned k, int vec[], int n = M) {
    for (int i = 0; i < n; ++i) {
        vec[i] = k & 1;
        k >>= 1;
    }
}

//实现多项式乘法
vector<int> multi(int a1[], int a2[]) {
    vector<int> res(2 * M - 1, 0);
    for (int i = 0; i < M; i++) {
        if (a1[i])
            for (int j = 0; j < M; ++j) {
                res[i + j] ^= a2[j];
            }
    }
    return res;
}

unsigned marge(vector<int> &arr) {
    unsigned res = 0;
    for (int i = M - 1; i >= 0; --i) {
        res ^= arr[i];
        res <<= 1;
    }
    return res >> 1;
}

void div(vector<int> &arr, const int poly[]) {
    int n = arr.size() - 1;
    while (n >= M) {
        if (arr[n]) {
            for (int i = 0; i < M; i++) {
                arr[n - M + i] ^= poly[i];
            }
        }
        n--;
    }
}

unsigned gf28_mul(unsigned a1, unsigned a2) {
    int arr1[M], arr2[M];
    transform(a1, arr1);
    transform(a2, arr2);

    vector<int> mul = multi(arr1, arr2);
    div(mul, poly);
    unsigned res = marge(mul);
    return res > 255 ? res ^ 0x11b : res;
}
