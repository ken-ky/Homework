// NFA_model.cpp
#include "NFAmodel.h"

unordered_map<char, int> op_map;

void init_map() {
    char arr[] = {'|', '(', ')', '*', '?', '.'};
    for (char i: arr) op_map[i]++;
}

bool isOperator(char op) {
    switch (op) {
        case '*':
        case '|':
        case '.':
        case '?':
            return true;
        default:
            return false;
    }
}

string rebuild(string &s) {
    if (s.empty()) return "";
    string new_s = "";
//    cout<<s.length()<<endl;
    new_s += s[0];
    for (int i = 1; i < s.length(); ++i) {
        if ((!op_map.count(s[i - 1]) && !op_map.count(s[i])) ||         //连续两个都不属于符号
            (s[i - 1] == ')' && !op_map.count(s[i])) || (!op_map.count(s[i - 1]) && s[i] == '(') ||    //有括号情况处理
            ((s[i - 1] == '*' || s[i - 1] == '?') && !op_map.count(s[i]))
                ) {
//            cout << s[i - 1] << "," << s[i] << endl;
            new_s += ".";
        }
        new_s += s[i];
    }
    op_map['?']++;
    op_map['*']++;
    return new_s;
}

int getPriority(int op) {
    int level = 0;  //设置连接运算的优先级
    switch (op) {
        case '(':
            level = 1;
            break;
        case '|':
            level = 2;
            break;
        case '.':
            level = 3;
            break;
        case '*':
        case '?':
            level = 4;
            break;
        default:
            break;
    }
    return level;
}

string toSuffix(const string &expr) {
    if (expr.empty()) return "";
    stack<char> op;
    string suffix;
    for (auto c: expr) {
        if (isOperator(c)) {
            if (op.empty()) op.emplace(c);
            else {
                while (!op.empty()) {
                    int t = op.top();
                    if (getPriority(c) <= getPriority(t)) {
                        op.pop();
                        suffix.push_back(t);
                    } else break;
                }
                op.emplace(c);
            }
        } else {
            if (c == '(') op.emplace(c);
            else if (c == ')') {
                while (op.top() != '(') {
                    suffix.push_back(op.top());
                    op.pop();
                }
                op.pop();
            } else suffix.push_back(c);
        }
    }
    while (!op.empty()) {
        suffix.push_back(op.top());
        op.pop();
    }
    return suffix;
}

//NFA 定义部分
NFA::NFA() {
    st = 0;
    ed = 0;
    graph.resize(1);
}

unordered_map<int, vector<int>> &NFA::operator[](int n) {
//    cout<<n<<endl;
    if (n > graph.size()) {
        cerr << "The index is out of size of this graph!" << endl;
        exit(2);
    }
    return graph[n];
}

NFA &NFA::operator+=(NFA &other) {
    int size = this->size();
    for (int i = 0; i < other.size(); ++i) {
        graph.emplace_back(other[i]);   //将图结构加入this中

        for (auto &j: graph.back())
            for (auto &k: j.second) k += size;  //对节点值进行重写
    }
    return *this;
}




