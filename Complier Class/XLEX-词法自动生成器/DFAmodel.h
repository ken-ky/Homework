//
// Created by ASUS on 2023/10/22.
//

#ifndef EXPERIMENT01_DFAMODEL_H
#define EXPERIMENT01_DFAMODEL_H

#include "NFAmodel.h"

//重构NFA生成DFA图形态
struct DStat {
    set<int> stats;
    bool end_flag;  //终结符号

    bool operator<(const DStat &d) const { return stats < d.stats; }

    bool operator==(const DStat &d) const { return stats == d.stats; }

    DStat() : end_flag(false) {}

    void judge_end(int n);  //判断是否为终态
};

struct DFA {
    map<DStat, vector<set<int>>> graph;
};

struct Min_DFA {
    map<set<int>,int> stat;
    set<int> final_stat;    //使用set<int>->val对应的状态号来确定是否为终态

    map<int,vector<int>> stat_map;   //程序状态图表

    explicit Min_DFA(DFA &b);
};

#endif //EXPERIMENT01_DFAMODEL_H
