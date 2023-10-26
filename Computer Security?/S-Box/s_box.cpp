#include <iostream>
#include <vector>
#include "gf.h"

using namespace std;

//对矩阵进行显示
void display(unsigned *arr[]) {
    for (int i = 0; i < 2 * M; ++i) {
        printf("[0x%02x", arr[i][0]);
        for (int j = 1; j < 2 * M; ++j) {
            printf(", 0x%02x", arr[i][j]);
        }
        printf("]\n");
    }
}

//生成初始矩阵
void init(unsigned *arr[]) {
    for (int i = 0; i < 2 * M; i++) {
        arr[i] = new unsigned[2 * M];
        for (int j = 0; j < 2 * M; ++j) {
            arr[i][j] = 2 * M * i + j;
        }
    }
}

//获取数字位数
int get_num(unsigned num) {
    int i = 0;
    while (num > 0) {
        num >>= 1;
        i++;
    }
    return i;
}

//求逆元函数
unsigned Euclid1(unsigned lower) {
    //当数字为{00}时返回{00}作为其逆元
    if (lower == 0) return 0;

    unsigned num = 0x11b;
    int v0 = 1, w0 = 0, v1 = 0, w1 = 1;
    int q;
    while (lower != 1) {
        q = get_num(num) - get_num(lower);
        if (q > 0) {
            num ^= (lower << q);
            v0 ^= (v1 << q);
            w0 ^= (w1 << q);
        } else {
            num ^= lower;
            v0 ^= v1;
            w0 ^= w1;
        }
        if (num < lower) {
            swap(v0, v1);
            swap(w0, w1);
            swap(num, lower);
        }
//        printf("v0=0x%02x, w0=0x%02x, r0=0x%02x\n", v0, w0, num);
//        printf("v1=0x%02x, w1=0x%02x, r1=0x%02x\n", v1, w1, lower);
        if (!lower) return w1;
    }
    return w1;
}

int **get_MatrixB(unsigned bi = 0x1f) {
    int **arr = new int *[M], temp;
    for (int i = 0; i < M; ++i) {
        arr[i] = new int[M];
    }

    transform(bi, arr[0], M);

    //进行移位生成新的矩阵{bi'}
    for (int i = 1; i < M; i++) {
        temp = arr[i - 1][M - 1];
        for (int j = 1; j < M; ++j) {
            arr[i][j] = arr[i - 1][j - 1];
        }
        arr[i][0] = temp;

//        for (int j = 0; j < M; ++j) {
//            printf("%d ",arr[i][j]);
//        }
//        printf("\n");
    }
    return arr;
}

//将逆元k转换为对应的矩阵数
void encode(int **arr, unsigned &k) {
    int a[M];
    transform(k, a, M);

    vector<int> res(M);
    for (int i = 0; i < M; ++i) {
        if (a[i]) {
            for (int j = 0; j < M; ++j) {
                res[j] ^= arr[i][j];
            }
        }
    }
    k = marge(res);
}

//进行矩阵逆元化
void rebuild_matrix(unsigned *arr[], unsigned bi = 0x1f, unsigned b = 63) {
    unsigned temp;
    int **MatrixB = get_MatrixB(bi);
    for (int i = 0; i < 2 * M; ++i) {
        for (int j = 0; j < 2 * M; ++j) {
            //矩阵逆元化
            temp = Euclid1(arr[i][j]);

            encode(MatrixB, temp);

            //进行最后的异或处理
            temp ^= b;
            arr[i][j] = temp;
        }
    }
}

int main() {
//    int k = 0x0c;
//    unsigned res = Euclid1(k);
//    int **arr = get_MatrixB();
//    encode(arr, res);
//    printf("res = 0x%02X\n", res);
//    printf("result = 0X%X", gf28_mul(k, res));  //0x80

    unsigned *box[2 * M];

    //初始化s_box
    init(box);
    rebuild_matrix(box);
    cout << endl;
    display(box);
    return 0;
}