// NFA_model.h
#ifndef NFA_MODEL_H
#define NFA_MODEL_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <algorithm>

using namespace std;

//包含括号的符号表（但不包括连接运算符）
extern unordered_map<char, int> op_map;

void init_map();

//转换为后缀表达式
bool isOperator(char op);   //判断是否为运算符

string rebuild(string &s);    //将连接运算符插入

int getPriority(int op);    //运算符优先级计算

string toSuffix(const string &expr);

struct NFA {
    int st;
    int ed;
    vector<unordered_map<int, vector<int>>> graph;

    NFA();

    ~NFA() {}

    int size() { return graph.size(); }

    unordered_map<int, vector<int>> &operator[](int n);

    NFA &operator+=(NFA &b);
};
#endif
