#ifndef EXPERIMENT01_REMODEL_H
#define EXPERIMENT01_REMODEL_H

#include "DFAmodel.h"

//正则表达式部分
class RE_Exp {
public:
    string exp;
    string tmp;

    //NFA构造部分
    int width;
    NFA nfa_exp;
    unordered_map<char, int> table;
    vector<char> title;

    void init_title();

    void build_NFA();    //生成此表达式对应的NFA内容

    explicit RE_Exp(string e1 = "") : exp(e1) {
//        cout<<"0"<<e1<<endl;
        width = 0;
        min_exp = nullptr;
        if (!e1.empty()) {
//            cout<<"1 ";
            tmp = rebuild(exp);
//            cout<<"1"<<tmp<<endl;
            tmp = toSuffix(tmp);
        } else tmp = "";   //使用空字符进行占位
    }

    void NFA_shown();

    //构造DFA图系统
    DFA dfa_exp;

    DStat e_closure(int n);

    DStat e_closure(DStat S);

    DStat merge(const DStat &S, int a);  //合并集合

    void build_DFA();

    void DFA_shown();

    //构造最小DFA
    Min_DFA *min_exp;

    void build_Stat();

    void min_shown();

    void merge_row(map<int, vector<int>> &m,int x,int y);

    void merge_stat();

    string generate_Code();

    ~RE_Exp() {}

    void initial_exp(); //对正则表达式进行初始化

    void shown(int n);
};

#endif //EXPERIMENT01_REMODEL_H
