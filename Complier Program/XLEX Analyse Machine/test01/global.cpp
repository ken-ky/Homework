#include "global.h"

/**     基本函数    **/


/**     引入全局变量      **/
unordered_map<char, int> opMap;

/***    自定义数据类型函数  ***/
void initOP() {
    char op[] = {'&', '|', '*', '?', '+'};
    for (char c : op) {
        switch (c) {
        case '|':
            opMap[c] = 1;
            break;
        case '&':
            opMap[c] = 2;
            break;
        case '*':
        case '+':
        case '?':
            opMap[c] = 3;
            break;
        default:
            cerr << "Error: input operator!" << endl;
            return;
        }
    }
}

int precedence(char op) {
    if (opMap.find(op) != opMap.end())
        return opMap[op];
    return 0;
}

string rebuild(string &s) {     //插入&符号
    if (s.empty()) return "";
    string new_s = "";  //l(l|d)*
    new_s += s[0];
    for (int i = 1; i < s.length(); ++i) {
        if ((precedence(s[i - 1]) == 3 && opMap.find(s[i]) == opMap.end() && s[i] != ')') ||
                (s[i - 1] == ')' && s[i] == '(') ||
                (s[i - 1] == ')' && opMap.find(s[i]) == opMap.end()) ||
                ((opMap.find(s[i - 1]) == opMap.end() && s[i - 1] != '(') &&
                 (opMap.find(s[i]) == opMap.end() && s[i] != ')')) ||
                ((opMap.find(s[i - 1]) == opMap.end() && s[i - 1] != ')') && s[i] == '(')
           ) {
            new_s += '&';
            new_s += s[i];
        } else new_s += s[i];
    }
    cout << new_s << endl;
    return new_s;
}

bool isOperator(char op) {
    switch (op) {
    case '?':
    case '*':
    case '+':
    case '|':
    case '&':
        return true;
    default:
        return false;
    }
}

string transAfter(string &infix) {
    initOP();
    string s = rebuild(infix);

    if (s.empty())
        return "";
    stack<char> op;
    string suffix;
    for (auto c : s) {
        if (isOperator(c)) {
            if (op.empty())
                op.emplace(c);
            else {
                while (!op.empty()) {
                    int t = op.top();
                    if (precedence(c) <= precedence(t)) {
                        op.pop();
                        suffix.push_back(t);
                    } else break;
                }
                op.emplace(c);
            }
        } else {
            if (c == '(')
                op.emplace(c);
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

/**     NFA部分      **/
nfaNode &nfaGraph::operator[](int n) {
    if (n > size()) {   //可能存在out-of-index情况
        cerr << "The index is out of size of this graph!" << endl;
        exit(2);
    }
    return states[n];
}

nfaGraph &nfaGraph::operator+=(nfaGraph &other) {
    int ed = this->size();

    auto otherLen = other.states.size();
    for (auto i = 0; i < otherLen; ++i) {
        other[i].id += ed;
        for (auto &j : other[i].edges) {
            j.EdState += ed;
        }
        this->states.emplace_back(other[i]);    //添加状态，已经改变了other.states
    }

    //另外还需要解决终态问题
    return *this;
}


/**     DFA部分   **/
void setDis(const set<int> &s) {
    cout << "{";
    for (auto &i : s)
        cout << i << ",";
    cout << "}" << endl;
}

set<int> nfaGraph::getTransStates(int curState, char input) {
    set<int> res = {};
    for (auto &edge : states[curState].edges) {
        if (edge.symbol == input)
            res.insert(edge.EdState);
    }

//    setDis(res);
    return res;
}

/**     DFA部分   **/
set<int> epsilonClosure(nfaGraph &nfa, const set<int> &states) {
    set<int> closure = states;
    queue<int> workList;

    //将起始状态放入
    for (auto state : states)
        workList.push(state);

    while (!workList.empty()) {
        int curState = workList.front();
        workList.pop();

        //形成
        auto tmp = move(nfa.getTransStates(curState, '#'));
        if (!tmp.empty())
            for (int nState : tmp) {
                if (closure.find(nState) == closure.end()) {
                    closure.insert(nState);
                    workList.push(nState);
                }
            }
    }
    return closure;
}

set<int> nfaMove(nfaGraph &nfa, const set<int> &states, char input) {
    set<int> moveStates;
    for (int state : states) {
        for (int nState : nfa.getTransStates(state, input)) {
            moveStates.insert(nState);
        }
    }
    return moveStates;
}

dfaGraph::dfaGraph(unordered_map<char, int> &chart, nfaGraph &nfa) {
    //初始化charSet
    for (auto &[c, i] : chart)
        if (c != '#')
            charSet[c] = i;

    //确定DFA的起始状态
    StStates = epsilonClosure(nfa, {0});
    dfaStates.insert(StStates);

    //终态操作
    finState = nfa.EdState;

    //子集构造法
    queue<set<int>> workList;

    workList.push(StStates);
    visited.insert(StStates);
    while (!workList.empty()) {
        set<int> curState = workList.front();
        for (auto &[c, _] : chart) {
            if (c != '#') {     //不是空边的起始状态
                set<int> nState = move(nfaMove(nfa, curState, c));
                auto tmp = epsilonClosure(nfa, nState);

                while (nState != tmp) {     //发生改变时就不停止空闭包
                    nState.insert(tmp.begin(), tmp.end());   //进行合并
                    tmp = epsilonClosure(nfa, nState);
                }
                transStates[curState][c] = nState;
                if (!nState.empty() && visited.find(nState) == visited.end()) {
                    workList.push(nState);
                    visited.insert(nState);
                }
            }
        }
        workList.pop();
    }

    //进行长度记录
    width = chart.size() - 1;   //除去空边的长度
//    cout << width << endl;  //2
    length = transStates.size();    //对应状态转移行数

    //寻找终态
    for (auto &state : visited) {
        if (state.find(finState) != state.end())
            finStates.insert(state);
    }
}

/**     minDFA部分    **/
bool vecEqual(vector<int> &a, vector<int> &b, int beg) {
    auto size = a.size();
    bool flag = true;
    for (int i = beg; i < size; ++i) {
        if (a[i] != b[i])
            flag = false;
    }
    return flag;
}

bool vecEmpty(vector<int> &a, int beg) {
    auto size = a.size();
    bool flag = true;
    for (int i = beg; i < size; ++i) {
        if (a[i] != 0)
            flag = false;
    }
    return flag;
}

void updateMatrix(vector<vector<int>> &states, int low, int gre, int beg) {
    for (auto &line : states)
        for (int i = beg; i < line.size(); ++i) {   //从beg开始进行列的置换工作
            if (line[i] == gre)
                line[i] = low;
        }
}

minDFA::minDFA(dfaGraph &dfa) {
    charSet = dfa.charSet;

//    cout<<endl;
//    for (auto [k, v]: charSet) {
//        cout<<k<<"\t";
//    }
//    cout<<endl;

    //生成状态等价组id表，初始化终态id集合
    int id = 1;
    int edDFA = dfa.finState;
//    cout << edDFA << endl;    //3
    stateTab[ {}] = 0; //空集设为0
    for (auto &state : dfa.visited) {
        stateTab[state] = id;
        if (state.find(edDFA) != state.end())
            finTab.insert(id);
        id++;
    }

//    setDis(finTab);   //{2,}:{},{}

    auto *dfaTab = &dfa.transStates;    //使用别名指向dfa状态转移表
    for (auto &[kSet, vSet] : *dfaTab) {
        vector<int> line(dfa.width + 1, 0);       //分布：stStateId, edStateId[0], ..., edStateId[n-1]
        line[0] = stateTab[kSet];

        for (auto &[c, valueSet] : vSet) {  //填充对应的状态转移等价组id
            line[dfa.charSet[c] + 1] = stateTab[valueSet];
        }
        minTransTab.push_back(line);
    }

    minimizeDFA(minTransTab, stateTab, finTab);
}

void minimizeDFA(vector<vector<int>> &states, unordered_map<set<int>, int> &stateTab, set<int> &finTab) {
    if (states.empty()) {
        cerr << "Error: DFA graph is empty." << endl;
        exit(2);
    }

    auto len = states.size() - 1;
    int tmp = 0;
    while (tmp != len) {
        tmp = len;

        for (int i = len; i > 0; --i) {
            for (int j = i - 1; j >= 0; --j) {
                if (i != j && vecEqual(states[i], states[j], 1)) {
                    auto low = states[j][0], gre = states[i][0];
                    states.erase(states.begin() + i);


                    updateMatrix(states, low, gre, 0);  //由line[0]开始转换

                    //若是合并状态是终态
                    if (finTab.find(gre) != finTab.end()) {
                        finTab.insert(low);
                        finTab.erase(gre);
                    }
                    break;
                }
            }
        }
        len = states.size() - 1;
    }
}

/**     RE部分    **/
/*      NFA部分   */
void reEXP::nfaDis() {
    cout << "St" << "\t" << "C" << "\t" << "Ed" << endl;
    for (int i = 0; i < nfa.size(); ++i) {
        for (int j = 0; j < nfa[i].edges.size(); ++j) {
            cout << nfa[i].id << "\t" << nfa[i].edges[j].symbol << "\t" << nfa[i].edges[j].EdState << endl;
        }
    }
}

void reEXP::buildNFA() {
    nfaFlag = 1;
    if (exp.empty()) {
        nfa.addEdge(0, 1);
        nfa.states.emplace_back(1);
        return;
    }

    stack<nfaGraph> sta;
    for (int i = 0; i < exp.length(); ++i) {
        if (!precedence(exp[i])) {
            nfaGraph a;
            a.addEdge(0, 1, exp[i]);
            a.states.emplace_back(1);
//            cout << a.size() << endl;   //输出：2\n 存在一个空状态

            sta.emplace(a);
        } else {
            switch (exp[i]) {
            case '&': {
                if (sta.size() < 2) {
                    cerr << "Exp error: " << exp << endl;
                    exit(2);
                }
                nfaGraph b = sta.top();
                sta.pop();
                nfaGraph a = sta.top();
                sta.pop();

                /*  连接：ab状态间需添加一条空边  */
                auto aEnd = a.size();
//                    cout << aEnd << endl;
                a.states[aEnd - 1].edges.emplace_back(aEnd);

                a += b;
                sta.emplace(a);
                break;
            }
            case '|': {
                if (sta.size() < 2) {
                    cerr << "Exp error: " << exp << endl;
                    exit(2);
                }
                nfaGraph b = sta.top();
                sta.pop();
                nfaGraph a = sta.top();
                sta.pop();

                /*  选择：需要添加两个空状态（开头和结尾） */
                nfaGraph c;
                c.addEdge(0, 1);
                c += a;
                int upPos = c.size() - 1;   //第一分支末状态id

                c.addEdge(0, upPos + 1);
                c += b;
                int downPos = c.size() - 1; //第二分支末状态id
                c.addEdge(upPos, downPos + 1);
                c.addEdge(downPos, downPos + 1);
                c.addState();   //添加尾状态节点

                sta.emplace(c);
                break;
            }
            case '*': {
                if (sta.empty()) {
                    cerr << "Exp error: " << exp << endl;
                    exit(2);
                }

                nfaGraph a = sta.top();
                sta.pop();

                /*  闭包：增加尾->头的空边，添加头尾状态，添加头->尾的空边 */
                auto aEnd = a.size();
                a.addEdge(aEnd - 1, 0);

                a.addEdge(aEnd - 1, aEnd);
                a.addState();

                nfaGraph head;
                head.addEdge(0, 1);
//                    cout << head.size() << endl;

                head += a;
                head.addEdge(0, head.size() - 1);

                sta.emplace(head);
                break;
            }
            case '?': {
                if (sta.empty()) {
                    cerr << "Exp error: " << exp << endl;
                    exit(2);
                }

                /*  可选：在头节点添加头->尾的空边    */
                nfaGraph a = sta.top();
                a.addEdge(0, a.size() - 1);
                break;
            }
            case '+': {
                if (sta.empty()) {
                    cerr << "Exp error: " << exp << endl;
                    exit(2);
                }

                nfaGraph a = sta.top();
                sta.pop();

                /*  正闭包：增加尾->头的空边，添加头尾状态 */
                auto aEnd = a.size();
                a.addEdge(aEnd - 1, 0);

                a.addEdge(aEnd - 1, aEnd);
                a.addState();

                nfaGraph head;
                head.addEdge(0, 1);
//                    cout << head.size() << endl;

                head += a;

                sta.emplace(head);
                break;
            }
            default: {
                cerr << "Function lack: " << exp[i] << endl;
                break;
            }
            }
        }
    }
    nfa = sta.top();

    //添加终态标识
    int ed = nfa.size() - 1;
    nfa[ed].isEnd = true;
    nfa.EdState = ed;
}

void reEXP::nfaTabInit() {
    nfaFlag == 0 ? (buildNFA()) : ({});
    nfaTabFlag = 1;    //初始化标识

    //初始化title
    int len = 0;
    for (char c : exp) {
        if (precedence(c) == 0 && title.find(c) == title.end()) {
            title[c] = len;
            len++;
        }
    }
    title['#'] = len;

    int l = nfa.size();
    auto w = title.size();
    width = w;
    for (int i = 0; i < l; ++i) {   //按状态起点遍历
        vector<vector<int>> line(width);
        auto &ptr = nfa[i];
        for (auto &edge : ptr.edges) {
            auto c = edge.symbol;
            if (title.count(c)) {
                line[title[c]].push_back(edge.EdState);
            }
        }
        nfaTable.emplace_back(line);
    }
}

void reEXP::nfaTableDis() {
    nfaTabFlag == 0 ? (nfaTabInit()) : ({});
    vector<char> vec(width);
    for (auto &[k, v] : title) {
        vec[v] = k;
    }

    cout << "ed\trw\t";
    for (int i = 0; i < width; ++i) {
        cout << vec[i] << "\t";
    }
    cout << endl;

    for (int i = 0; i < nfaTable.size(); ++i) {
        cout << nfa[i].isEnd << "\t" << i << "\t";
        for (auto &v : nfaTable[i]) {
            if (v.empty()) cout << " \t";
            else {
                for (auto &k : v)
                    cout << k << ",";
                cout << "\t";
            }
        }
        cout << endl;
    }
}


/*      DFA部分  */
void reEXP::dfaTableDis() {
    dfaFlag == 0 ? (dfaInit()) : ({});

    int row = 0;
    for (auto &[StStates, vStates] : dfa->transStates) {
        cout << ((StStates.find(dfa->finState) != StStates.end()) ? "+" : "") << "\t" << row << "\t{";
        for (int state : StStates)
            cout << state << ",";
        cout << "}:\t";
        for (auto &[k, EdStates] : vStates) {
            cout << k << ":{";
            for (int state : EdStates) {
                cout << state << ",";
            }
            cout << "}\t";
        }
        cout << endl;
        row++;
    }
}

/*      minDFA部分    */
void reEXP::minDfaTableDis() {
    minDfaFlag == 0 ? (minDfaInit()) : ({});
//    dfaTableDis();
//    cout << endl;

    vector<char> vec(width - 1);
    for (auto &[k, v] : minDfa->charSet) {
        if (v != '#')
            vec[v] = k;
    }
    cout << " \tst\t";
    for (int i = 0; i < width - 1; ++i) {
        cout << vec[i] << "\t";
    }
    cout << endl;

    //遍历等价态
    for (auto &line : minDfa->minTransTab) {
        cout << (minDfa->finTab.find(line[0]) != minDfa->finTab.end()) << "\t";
        for (auto state : line) {
            cout << state << "\t";
        }
        cout << endl;
    }
}

/**     生成代码函数部分    **/
/*      文本文件处理  */
vector<string> getTxt(string addr) {
    fstream op(addr, ios::in);
    vector<string> txt;
    string line;
    while (getline(op, line)) {
        if (line.substr(0, 2) == "//" || line.empty())  //出现注释符号在line[0,2)位置时，直接跳过
            continue;
//        cout << line << endl;
        txt.emplace_back(line);
    }
    op.close();
    return txt;
}

//生成[id,正则式]的数组，同时更新[类型,id]的哈希表
vector<string> proRE(vector<string> &vec, vector<string> lines, vector<string> &words) {
    vector<string> reStr;   //对应的正则表达式

    int id = 0;
    for (auto &line : lines) {
        auto mid = line.find(' ');
        auto size = line.size();

        auto word = line.substr(mid + 1, size - mid - 1);
        int i;
        for (i = 0; i < word.length(); i++) {
            if (!isalpha(word[i]))
                break;
        }
        if (i == word.length())
            words.emplace_back(word);
        vec.push_back(line.substr(0, mid));
        reStr.push_back(word);
    }
    return reStr;
}

//截取某个字符最后一次出现的位置
int getLastChar(string str, const char &c) {
    auto len = str.length();

    int pos;
    for(int i = 0; i < len; i++)
        if(str[i] == c)
            pos = i;
    return pos;
}

vector<string> splitLine(string &line, char sep) {
    vector<string> vecArr;
    int flagSub = 0;
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == sep) {
            string temp = line.substr(flagSub, i - flagSub);
            vecArr.push_back(temp);
            flagSub = i + 1;
        }
    }
    vecArr.push_back(line.substr(flagSub, line.length() - flagSub));
    return vecArr;
}

/*      代码产生结构体 */
//子程序生产程序
string codePro::proSubCode(reEXP *re, int id) {
    string str = "";
    str += "bool strMatch" + to_string(id)
           + "(const string &str) {\n"
           "    auto size = str.length();   //Len of a word\n"
           "";

    //生成字符数组如：char tab[] = {'>', '='};
    vector<char> vec(re->width - 1);
//    cout << endl;
    for (auto &[k, v] : re->minDfa->charSet) {
//        cout << k << "\t";
        if (k != '#')
            vec[v] = char(k);
    }
//    cout << endl;

//    for (auto v: vec) {
//        cout << v << "\t";
//    }
//    cout << endl;

    str += "    char tab[] = {\'";
    str += vec[0];
    str += "\'";
    for (int i = 1; i < vec.size(); ++i) {
        str += ",\'";
        str += vec[i];
        str += "\'";
    }
    str += "};\n";

    //植入最小化DFA矩阵
    str += "\n"
           "    map<int, vector<int>> arr = {";
    auto dfa = re->minDfa;
    if (dfa->minTransTab.empty()) {
        cerr << "minDFA Graph lack!" << endl;
        exit(3);
    }

    auto line = dfa->minTransTab[0];
    auto width = line.size();

    //首行，但是单行结构为：{bState,{eState0,eState1,...}}
    str += "{" + to_string(line[0]) + ",";  //键
    str += "{" + to_string(line[1]);
    for (int i = 2; i < width; ++i) {
        str += "," + to_string(line[i]);
    }
    str += "}}";

    //其他行
    bool flag = false;
    for (int i = 1; i < dfa->minTransTab.size(); ++i) {
        flag = true;
        line = dfa->minTransTab[i];
        str += ",{" + to_string(line[0]) + ",";
        str += "{" + to_string(line[1]);
        for (int j = 2; j < width; ++j) {
            str += "," + to_string(line[j]);
        }
        str += "}}";
    }
    str += "};\n";


    //寻找开始状态
    set<int> idSet;
    str += "    int state = ";
    int stId;
    for (auto &[key, val] : dfa->stateTab) {
        if (key.find(0) != key.end())
            stId = val;
        if (val != 0)
            idSet.insert(val);
    }
    if (!flag)
        str += to_string(1) + ";\n"
               "\n";
    else
        str += to_string(stId) + ";\n"
               "\n";

    str += "    auto colLen = arr[state].size();\n"
           "\n";

    //进行状态转换代码生成
    str += "    int pos = 0;\n"
           "    while (pos < size) {\n"
           "        bool flag = false;\n"
           "        switch (state) {\n";
    for (auto &item : idSet) {
        str += "            case " + to_string(item) + ":\n";
        str += "                state = transState(str, flag, pos, arr[state], tab, colLen);\n";
        str += "                break;\n";
    }
    str += "            default:\n"
           "                return false;\n"
           "        }\n"
           "        pos++;\n"
           "    }\n";

    //设置终态结束部分
    str += "\n"
           "    if (state != ";
    int times = 0;
    for (auto &id : dfa->finTab) {
        if (times > 0) {
            str += " && state != ";
        }
        str += to_string(id);
        times++;
    }
    str += ")\n"
           "        return false;\n"
           "    return true;\n"
           "}\n\n";
//    cout << str << endl;
    return str;
}

void codePro::initCode(string inAddr, string outAddr) {
    int times = 0;
    string tmp;

    //批量生成子程序队列
    for (auto re : REs) {
        tmp += proSubCode(re, times++);
    }

    //生成调用子程序队列的大型程序
    if (reStr.empty()) {
        cerr << "reStr is empty!" << endl;
        exit(3);
    }
    tmp += "string analyse(vector<string> &words, map<char, char> &backC) {\n"
           "    vector<string> typeStr={\"" + reStr[0] + "\"";
    for (int i = 1; i < reStr.size(); ++i) {
        tmp += ",\"" + reStr[i] + "\"";
    }
    tmp += "};\n"
           "    string res;\n"
           "    for (auto &word: words) {\n"
           "        if (strMatch0(word)) {\n"
           "            res += word + \" \";\n"
           "            res += typeStr[0] + \"\\n\";\n"
           "        } ";
    for (int i = 1; i < reStr.size(); ++i) {
        tmp += "else  if (strMatch" + to_string(i) + "(word)) {\n";
        tmp += "            res += word + \" \";\n";
        tmp += "            res += typeStr[" + to_string(i) + "] + \"\\n\";\n";
        tmp += "        } ";
    }
    tmp += "else if (word == \"(\") {\n"
           "            res += word + \" \";\n"
           "            res += \"LEC\\n\";\n"
           "        } else if (word == \")\") {\n"
           "            res += word + \" \";\n"
           "            res += \"REC\\n\";\n"
           "        } ";
    tmp += "else {\n"
           "            res += word + \" ERR\\n\";\n"
           "        }\n";
    tmp += "    }\n"
           "    transChar(res, backC);\n"
           "    return res;\n"
           "}\n"
           "\n";

    codeStr += tmp;   //将此结果累加到之前的输出中

    /*  生成main函数    */
    codeStr += "int main() {\n";

    //生成第一个map：reC
    string mapStr;
    mapStr += "    map<char, char> reC = {";
    for (int i = 0; i < charTab.size(); i += 2) {
        mapStr += "{\'";
        mapStr += charTab[i];
        mapStr += "\',\'";
        mapStr += charTab[i + 1];
        mapStr += "\'},";
    }
    mapStr.pop_back();  //删除最后一个字符
    mapStr += "};\n";
    codeStr += mapStr;


    codeStr += "    auto vec = getTny(\"" + inAddr + "\", reC);\n"
               "\n";

    //生成第二个map：backC
    mapStr = "";
    mapStr += "    map<char, char> backC = {";
    for (int i = 0; i < charTab.size(); i += 2) {
        mapStr += "{\'";
        mapStr += charTab[i + 1];
        mapStr += "\',\'";
        mapStr += charTab[i];
        mapStr += "\'},";
    }
    mapStr.pop_back();  //删除最后一个字符
    mapStr += "};\n";
    codeStr += mapStr;

    codeStr += "    saveLex(\"" + outAddr + "\", analyse(vec, backC));\n";
    codeStr += "    return 0;\n"
               "}\n";
}

void codePro::outCPP(std::string addr) {
    fstream op(addr, ios::out);
    op << codeStr;
    op.close();
    cout << endl << "Have saved the code!" << endl;
}

/**     编译代码部分  **/
bool compileCode(string inAddr, string outAddr) {   //out:*.exe in:*.cpp
    string str = "g++ -o" + outAddr + " " + inAddr;
    const char *cmd = str.data();

    //执行成功时，system返回0
    if (!system(cmd))
        return true;
    return true;
}

void runCode(string inAddr, string outAddr, string tny, string lex) {
    if (!compileCode(inAddr, outAddr)) {
        cerr << "Execute the code failed!" << endl;
        exit(-1);
    } else {
        const char *str = outAddr.data();
        system(str);

        cout << "Have parsed the code!" << endl;
    }
}
