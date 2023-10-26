#include <sstream>
#include "REmodel.h"
#include "DFAmodel.h"

void RE_ERROR(int Err_code) {
    if (Err_code == 1) {
        cerr << "Error, this is not a regular expression!" << endl;
        exit(1);
    }
    if (Err_code == 2) {
        cerr << "Error, the DFA is empty!!" << endl;
        exit(2);
    }
}

//进行NFA图化部分
void RE_Exp::init_title() {
    int j = 0;
    for (int i = 0; i < tmp.length(); i++)
        if (!table.count(tmp[i]) && !op_map.count(tmp[i])) {
            table[tmp[i]] = j++;
            title.push_back(tmp[i]);
        }
    table['#'] = j;
    title.push_back('#');
    width = title.size();
}

void RE_Exp::build_NFA() {
    if (tmp.empty()) {  //针对空串输入
        nfa_exp.ed = 1;
        nfa_exp.graph.resize(2);
        nfa_exp[0]['#'].emplace_back(1);
        return;
    }
    this->init_title();

    stack<NFA> sta;
    int end_op = table['#'];    //设置空串字符用于简化查询流程
    for (int i = 0; i < tmp.length(); ++i) {
        if (!isOperator(tmp[i])) {
            NFA a;
            a.graph.resize(2);
            a.ed = 1;
            a[0][table[tmp[i]]].push_back(a.ed);
            sta.emplace(a);
        } else {
            switch (tmp[i]) {
                case '.': {
                    if (sta.size() < 2) RE_ERROR(1);
                    NFA b = sta.top();
                    sta.pop();

                    NFA a = sta.top();
                    sta.pop();
                    a.graph.pop_back();

                    int ed1 = a.size();
                    a += b;
                    a.ed = b.ed + ed1;
                    sta.emplace(a);
                    break;
                }
                case '|': {
                    if (sta.size() < 2) RE_ERROR(1);
                    NFA b = sta.top();
                    sta.pop();

                    NFA a = sta.top();
                    sta.pop();

                    NFA temp;

                    int ed1 = temp.size(), ed2;
                    temp += a;
                    temp[temp.st][end_op].push_back(a.st + ed1);

                    ed2 = temp.size();
                    temp += b;
                    temp[temp.st][end_op].push_back(b.st + ed2);
//                    cout << b.st + ed2 << endl;
                    temp[a.ed + ed1][end_op].push_back(b.st + ed2);

                    temp[a.ed + ed1][end_op] = {temp.size()};
                    temp[b.ed + ed2][end_op] = {temp.size()};

                    temp.ed = temp.size();

                    temp.graph.emplace_back(unordered_map<int, vector<int>>({}));
                    sta.emplace(temp);
                    break;
                }
                case '*': {
                    if (sta.empty()) RE_ERROR(1);
                    NFA a = sta.top();
                    sta.pop();

                    NFA temp;
                    int ed1 = temp.size(), ed2 = a.size();
                    temp += a;

                    temp[0][end_op].push_back(ed1);

                    temp[ed1 + a.ed][end_op].push_back(a.st + ed1); //回环

                    temp.graph.emplace_back(unordered_map<int, vector<int>>({}));
                    temp[ed2][end_op].push_back(temp.size() - 1);
                    temp[0][end_op].push_back(temp.size() - 1);

                    NFA b;
                    b += temp;
                    b[0][end_op].push_back(ed1);
                    sta.emplace(b);
                    break;
                }
                case '?': {
                    if (sta.empty()) RE_ERROR(1);
                    NFA a = sta.top();
                    sta.pop();

                    NFA temp;
                    int ed1 = temp.size(), ed2 = a.size();
                    temp += a;

                    temp[0][end_op].push_back(ed1);

                    temp.graph.emplace_back(unordered_map<int, vector<int>>({}));
                    temp[ed2][end_op].push_back(temp.size() - 1);
                    temp[0][end_op].push_back(temp.size() - 1);

                    NFA b;
                    b += temp;
                    b[0][end_op].push_back(ed1);
                    sta.emplace(b);

                    sta.emplace(b);
                    break;
                }
            }
        }
    }
    nfa_exp = sta.top();
}

void RE_Exp::NFA_shown() {
    cout << "\t";
    for (int i = 0; i < width; i++) cout << "\t\t" << title[i];
    cout << endl;

    for (int i = 0; i < nfa_exp.size() - 1; i++) {
        if (!i) cout << "-";
        cout << "\t" << i + 1 << "\t\t";
        for (int j = 0; j < width; j++) {
            if (nfa_exp[i][table[title[j]]].empty()) cout << "\t\t";
            else {
                for (auto k: nfa_exp[i][table[title[j]]]) cout << k + 1 << ",";
                cout << "\t";
            }
        }
        cout << endl;
    }
    cout << "+\t" << nfa_exp.size() << endl;
}

//以下为合并NFA为DFA部分
DStat RE_Exp::e_closure(int n) {    //对单个数进行求闭包[单层步骤]
    DStat stat;
    stat.stats.insert(n);

    stack<int> sta;
    sta.emplace(n);

    int i, end_op = table['#'];
    while (!sta.empty()) {
        i = sta.top();
        sta.pop();
        for (auto &v: nfa_exp[i][end_op]) {    //进行闭包运算
            if (!stat.stats.count(v)) {  //已经存在此元素在集合中
                sta.emplace(v);
                stat.stats.insert(v);
            }
        }
    }
    return stat;
}

DStat RE_Exp::e_closure(DStat S) {  //对一个集合求闭包
    DStat stat;
    for (auto i: S.stats) {
        auto temp = e_closure(i);
        for (auto j: temp.stats)
            if (!stat.stats.count(j))  //已经存在此元素在集合中
                stat.stats.insert(j);
    }

//    for (auto k: stat.stats) cout << k + 1 << " ";
//    cout << endl;
    return stat;
}

DStat RE_Exp::merge(const DStat &S, int a) {
    DStat stat;
    for (auto &i: S.stats)
        if (!nfa_exp[i][a].empty())
            for (auto &v: nfa_exp[i][a])
                if (!stat.stats.count(v)) stat.stats.insert(v);
    return stat;
}

void RE_Exp::build_DFA() {
    int end = nfa_exp.size() - 1;

    //第0个节点
    queue<DStat> Dstats; //进行变量重命名

    auto temp = e_closure(nfa_exp.st);
    if (temp.stats.empty()) RE_ERROR(2);
    Dstats.push(temp);  //求取nfa头节点的闭包集合

    map<DStat, vector<set<int>>> &G = dfa_exp.graph;

    G[temp].resize(width - 1);

    for (auto &[t, _]: G) {    //进行逐行构造
        for (int i = 0; i < width - 1; i++) {
            auto s = e_closure(merge(t, i));
            auto &r = s.stats;
            if (!r.empty() && !G.count(s)) {
                G[s].resize(width - 1);
                s.judge_end(end);
                Dstats.push(s);
            }
            G[t][i] = r;
        }
        Dstats.pop();
    }
}

void RE_Exp::DFA_shown() {
    map<DStat, vector<set<int>>> &G = dfa_exp.graph;
    cout << "\t\t";
    for (int i = 0; i < width - 1; i++) cout << "\t\t\t" << title[i];
    cout << endl;

    for (auto &[k, v]: G) {
        cout << "{";
        for (auto &i: k.stats) cout << i + 1 << ",";
        cout << "}:";

        for (int i = 0; i < width - 1; i++) {
            if (v[i].empty()) cout << "{}\t\t";
            else {
                cout << "{";
                for (auto &j: v[i]) cout << j + 1 << ",";
                cout << "}\t";
            }
        }
        cout << endl;
    }
}

//最小DFA定义部分
void RE_Exp::build_Stat() {
    map<DStat, vector<set<int>>> &G = dfa_exp.graph;
    auto *Min_exp = new Min_DFA(dfa_exp);
    map<int, vector<int>> &min_map = Min_exp->stat_map;

    for (auto &[k, v]: G) {
        int i = Min_exp->stat[k.stats];
        min_map[i].resize(width - 1);
        for (int j = 0; j < width - 1; j++) {
            if (v[j].empty()) min_map[i][j] = -1;
            else min_map[i][j] = Min_exp->stat[v[j]];
        }
    }
    min_exp = Min_exp;
}

void RE_Exp::min_shown() {
    for (int i = 0; i < width - 1; i++) cout << "\t" << title[i];
    cout << endl;
    map<int, vector<int>> &min_map = min_exp->stat_map;
    for (auto &[k, v]: min_map) {
        cout << k + 1;
        for (int i = 0; i < width - 1; i++) cout << "\t" << v[i] + 1;
        cout << endl;
    }
}

void RE_Exp::merge_row(map<int, vector<int>> &m, int x, int y) {
    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < width - 1; j++) {
            if (m[i][j] == x) m[i][j] = y;
        }
    }
}

void RE_Exp::merge_stat() {
    map<int, vector<int>> &m = min_exp->stat_map;
    set<int> s;

    int index;
    for (int i = m.size() - 1; i > 0; --i) {
        index = -1;
        for (int j = i - 1; j >= 0; j--) {
            if (m[i] == m[j]) { //当两行向量相同
                index = j;
                s.insert(i);
                break;
            }
        }

        if (index == -1) continue;
        merge_row(m, i, index);
    }

    for (auto i: s) m.erase(i);
}

//生成代码的部分
string RE_Exp::generate_Code() {
    map<int, vector<int>> &m = min_exp->stat_map;
    int r = m.size();

    string s = "#include <iostream>\r\nusing namespace std;\r\n\r\n";

    s += "char edge[]={'";
    s += title[0];

    for (int i = 1; i < width - 1; ++i) {
        s += "','";
        s += title[i];
    }
    s += "'};\r\n";

    string n1, n2;
    stringstream ss;
    ss << m.size();

    n1 = ss.str();
    ss.str("");
    ss << width - 1;

    n2 = ss.str();
    ss.str("");

    s += "int DFA[" + n1 + "][" + n2 + "]={{";
    ss << m[0][0];

    s += ss.str();
    ss.str("");

    for (int i = 1; i < width - 1; ++i) {
        s += ",";
        ss << m[0][i];
        s += ss.str();
        ss.str("");
    }
    s += "}";

    for (int i = 1; i < r; ++i) {
        s += ",{";
        ss << m[i][0];
        s += ss.str();
        ss.str("");
        for (int j = 1; j < width - 1; ++j) {
            s += ",";
            ss << m[i][j];
            s += ss.str();
            ss.str("");
        }
        s += "}";
    }
    s += "};\r\n";

    s += "int getPos(char c) { \r\n  int i=0,pos=-1;\r\n  for(i=0;i<" + n2 +
         ";i++) {\r\n    if(edge[i]==c) pos=i;\r\n }\r\n return pos;\r\n}\r\n\r\n";
    s += "int main() {\r\n  int j=0,start=0,column;\r\n  char *str=new char[256];\r\n  cin>>str;\r\n  while(str[j]!=\'\\0\') {\r\n    switch(str[j]) {\r\n";

    string t_s = "";
    for (int i = 0; i < width - 1; ++i) {
        t_s = "";
        t_s += "      case'";
        t_s += title[i];
        t_s += "':\r\n        column=getPos(str[j]);\r\n        if(column!=-1&&start!=-1) start=DFA[start][column];\r\n";
        t_s += "        else {\r\n          cout<<\"NO\"<<endl;\r\n          return -1;\r\n        } \r\n        break;\r\n ";
        s += t_s;
    }
    s += "      default:\r\n        cout<<\"NO\"<<endl;\r\n        return -1;\r\n        break;\r\n    }\r\n    j++;\r\n  }";
    s += "\r\n  cout<<\"YES\"<<endl;\r\n  return 0;\r\n}";
    return s;
}

void RE_Exp::initial_exp() {
    this->build_NFA();
    this->build_DFA();
    this->build_Stat();
    this->merge_stat();
}

void RE_Exp::shown(int n) {
    switch (n) {
        case 0:
            this->NFA_shown();
            break;
        case 1:
            this->DFA_shown();
            break;
        case 2:
            this->min_shown();
            break;
        case 3:
            cout << this->generate_Code() << endl;
            break;
        case 1024:
            cout << tmp << endl;
            break;
        default:
            cerr << "Error, invalid code!" << endl;
            exit(404);
    }
}