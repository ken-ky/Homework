#ifndef TEST01_GLOBAL_H
#define TEST01_GLOBAL_H

//数据结构
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <functional>

using std::string;
using std::vector;
using std::set;
using std::stack;
using std::queue;
using std::unordered_set;
using std::unordered_map;
using std::move;    //用于移动的函数，不经过拷贝函数
using std::to_string;   //转换值到字符串
namespace std {
    template<>
    struct hash<std::set<int>> {
        size_t operator()(const std::set<int> &s) const {
            size_t hash = 0;
            for (int num: s) {
                hash ^= std::hash<int>{}(num); // 使用异或操作来混合哈希值
            }
            return hash;
        }
    };
}

//输入输出流
#include <fstream>
#include <iostream>

using std::ios;
using std::fstream;
using std::cout;
using std::cerr;
using std::endl;
using std::system;

/**     基本函数   **/
//插入&
string rebuild(string &s);

//中缀转后缀
string transAfter(string &vec);

/**     自定义数据类型及其相关函数 **/
/*  正则式运算符  */
extern unordered_map<char, int> opMap;

/*  NFA部分   */
//边定义
struct nfaEdge {
    char symbol;
    int EdState;

    explicit nfaEdge(int ed, char sym = '#') : EdState(ed), symbol(sym) {}
};

//状态节点定义
struct nfaNode {
    int id;     //对应此处的状态id
    bool isEnd;     //是否为终态节点
    vector<nfaEdge> edges;  //对应从此处出发的边

    explicit nfaNode(int st) : id(st), isEnd(false) {}

    void addEdge(int ed, char sym) {
        edges.emplace_back(ed, sym);
    }
};

//图定义
struct nfaGraph {
    vector<nfaNode> states;
    int EdState;     //终态对应id

    nfaGraph() {
        addState();
    }

    int addState() {
        int newState = states.size();
        states.emplace_back(newState);
        return newState;
    }

    void addEdge(int st, int ed, char sym = '#') {
        for (auto &node: states) {
            if (st == node.id) {
                node.addEdge(ed, sym);
            }
        }
    }

    int size() {    //return obj.size
        return states.size();
    }

    nfaNode &operator[](int n);

    nfaGraph &operator+=(nfaGraph &other);

    set<int> getTransStates(int curState, char input);  //epsilon可转移状态
};

/*  DFA部分   */
struct dfaGraph {
    int width;
    int length;
    unordered_map<char, int> charSet;
    unordered_map<set<int>, unordered_map<char, set<int>>> transStates; //状态转移表
    set<int> StStates;
    set<set<int>> dfaStates;
    unordered_set<set<int>> visited;

    int finState;    //终结状态集合
    set<set<int>> finStates;    //终态集合的集合

    explicit dfaGraph(unordered_map<char, int> &chart, nfaGraph &nfa);
};

void setDis(const set<int> &s);

set<int> epsilonClosure(nfaGraph &nfa, const set<int> &states);  //空闭包可达处

set<int> nfaMove(nfaGraph &nfa, const set<int> &states, char input);    //求经单步转移形成的状态集合

/*  minDFA部分    */
struct minDFA {
    unordered_map<char, int> charSet;
    unordered_map<set<int>, int> stateTab;  //等价状态对表：状态集合——等价组id
    set<int> finTab;    //终态等价组id表
    vector<vector<int>> minTransTab;    //目标生成的等价状态组转移表

    explicit minDFA(dfaGraph &dfa);
};

void minimizeDFA(vector<vector<int>> &states, unordered_map<set<int>, int> &stateTab, set<int> &finTab);  //使用合并法最小化

/*  RE部分   */
struct reEXP {
    string exp;
    int width;
    nfaGraph nfa;
    dfaGraph *dfa;
    minDFA *minDfa;

    //初始化NFA
    explicit reEXP(string e = "") : exp(e), width(0), dfa(nullptr), minDfa(nullptr) {
        if (!e.empty()) exp = transAfter(e);
        else exp = "";

        //初始化判定
        nfaFlag = 0;
        nfaTabFlag = 0;
        dfaFlag = 0;
        minDfaFlag = 0;
    }

    //NFA部分
    int nfaFlag;

    void nfaDis();  //展示NFA

    void buildNFA();    //构建NFA

    //规则化表
    unordered_map<char, int> title;
    vector<vector<vector<int>>> nfaTable;

    void nfaTabInit();

    //初始化nfa表相关内容
    int nfaTabFlag;

    void nfaTableDis();    //表格方式展示

    //DFA部分
    int dfaFlag;

    void dfaInit() {
        nfaTabFlag == 0 ? (nfaTabInit()) : ({});   //未初始化表就初始化

        dfaFlag = 1;
        dfa = new dfaGraph(title, nfa);
    }

    void dfaTableDis();

    //minDFA部分
    int minDfaFlag;

    void minDfaInit() {
        dfaFlag == 0 ? (dfaInit()) : ({});

        minDfaFlag = 1;
        minDfa = new minDFA(*dfa);
    }

    void minDfaTableDis();
};

/**      生成代码部分  **/
/*      文本文件处理    */
//读文件
vector<string> getTxt(string addr);

//RE正则表文件更新数据
vector<string> proRE(vector<string> &vec, vector<string> lines, vector<string> &words);

//返回某个字符最后一次出现的位置
int getLastChar(string str,const char &c);

//切割字符串
vector<string> splitLine(string &line, char sep);

/*      生成代码部分  */
struct codePro {
    vector<string> wordList;   //用来记录关键字区域

    vector<string> reStr;    //导入的类型对应表

    vector<reEXP *> REs;   //生成的正则表达式表

    string codeStr;   //程序前段的必要部分

    vector<char> charTab;   //需要转义的正则字符，及其预留的替代字符

    explicit codePro(string path) {
        auto exps = proRE(reStr, getTxt(path), wordList);

        /*  前段代码初始化 */
        //头文件声明
        codeStr = "#include <iostream>\n"
                  "#include <fstream>\n"
                  "\n"
                  "#include <string>\n"
                  "#include <vector>\n"
                  "#include <map>\n"
                  "#include <cctype>\n"
                  "\n";

        //std命名空间预载
        codeStr += "using std::cin;\n"
                   "using std::cout;\n"
                   "using std::endl;\n"
                   "using std::fstream;\n"
                   "using std::ios;\n"
                   "using std::getline;\n"
                   "\n"
                   "using std::string;\n"
                   "using std::vector;\n"
                   "using std::map;\n"
                   "using std::isalnum;\n"
                   "using std::isdigit;\n"
                   "using std::isalpha;\n"
                   "\n";

        //状态转换函数
        codeStr += "int transState(const string &str, bool &flag, int pos, vector<int> &arr, char tab[], size_t colLen) {\n"
                   "    for (int i = 0; i < colLen; ++i) {\n"
                   "        if (str[pos] == tab[i]) {\n"
                   "            flag = true;\n"
                   "            return arr[i];\n"
                   "        }\n"
                   "    }\n"
                   "    return 0;\n"
                   "}\n"
                   "\n";

        //分词函数
        codeStr += "vector<string> splitCode(string &line) {\n"
                   "    vector<string> words;\n"
                   "\n"
                   "    int pos = 0;\n"
                   "    auto len = line.size();\n"
                   "    string tmp;\n"
                   "    while (pos < len) {\n"
                   "        while (line[pos] == '\\n' || line[pos] == '\\t' || line[pos] == ' ')\n"
                   "            pos++;\n"
                   "        if (pos == len)\n"
                   "            break;\n"
                   "\n"
                   "        if (isalnum(line[pos]) || line[pos] == '_') {\n"
                   "            while (isalnum(line[pos]) || line[pos] == '_') {\n"
                   "                if (line[pos] == '\\n' || line[pos] == '\\t' || line[pos] == ' ')\n"
                   "                    break;\n"
                   "                tmp += line[pos++];\n"
                   "            }\n"
                   "        } else if (line[pos] != '(' && line[pos] != ')' &&\n"
                   "                   line[pos] != '[' && line[pos] != ']' &&\n"
                   "                   line[pos] != '{' && line[pos] != '}'\n"
                   "                ) {\n"
                   "            while (!isalnum(line[pos]) &&\n"
                   "                   line[pos] != '(' && line[pos] != ')' &&\n"
                   "                   line[pos] != '[' && line[pos] != ']' &&\n"
                   "                   line[pos] != '{' && line[pos] != '}'\n"
                   "                    ) {\n"
                   "                if (isalnum(line[pos]) || line[pos] == '_' || line[pos] == '\\n' || line[pos] == '\\t' ||\n"
                   "                    line[pos] == ' ')\n"
                   "                    break;\n"
                   "                tmp += line[pos++];\n"
                   "            }\n"
                   "        } else if (line[pos] == '(' || line[pos] == ')' ||\n"
                   "                   line[pos] == '[' || line[pos] == ']' ||\n"
                   "                   line[pos] == '{' || line[pos] == '}')\n"
                   "            tmp += line[pos++];\n"
                   "        else pos++;\n"
                   "        words.emplace_back(tmp);\n"
                   "        tmp = \"\";\n"
                   "    }\n"
                   "    words.pop_back();   //Pop the last one\n"
                   "    return words;\n"
                   "}\n"
                   "\n";

        //转义函数书写
        codeStr += "void transChar(string &word, map<char, char> &reC) {\n"
                   "    for (auto &c: word) {\n"
                   "        if (reC.find(c) != reC.end())\n"
                   "            c = reC[c];\n"
                   "    }\n"
                   "}\n"
                   "\n";

        //获取文件内容函数
        codeStr += "vector<string> getTny(string addr, map<char, char> &reC) {\n"
                   "    fstream op(addr, ios::in);\n"
                   "    string lines;\n"
                   "\n"
                   "    string line;\n"
                   "    while (getline(op, line)) {\n"
                   "        lines += line;\n"
                   "        lines += \"\\n\";\n"
                   "    }\n"
                   "    op.close();\n"
                   "    \n"
                   "\n"
                   "    transChar(lines, reC);"
                   "    return splitCode(lines);\n"
                   "}\n"
                   "\n";

        //存储文件内容函数
        codeStr += "void saveLex(string addr, string txt) {\n"
                   "    fstream op(addr, ios::out);\n"
                   "    op << txt;\n"
                   "    op.close();"
                   "}\n"
                   "\n";

        /*  生成代码预工作 */
        //批量生成最小化DFA
        for (auto &exp: exps) {
            auto tmp = new reEXP(exp);
//            cout << transAfter(tmp->exp).size() << endl;
            tmp->minDfaInit();
            REs.push_back(tmp);
            cout << "The DFA graph of " << exp << " has complete minDFA!" << endl;
//            tmp->minDfaTableDis();
        }

        //生成最小化DFA
//        auto tmp = new reEXP("repeat");
//        tmp->minDfaInit();
////        tmp->minDfaTableDis();
//        REs.push_back(tmp);
//
//        cout << proSubCode(*tmp,0) << endl;

        //进行字符转义表初始化
        char reC[] = {'&', '|', '*', '?', '+'};
        char changeC[] = {'.', '~', '^', '$', '@'};
        int size = sizeof(reC) / sizeof(reC[0]);
        for (int i = 0; i < size; ++i) {
            charTab.push_back(reC[i]);
            charTab.push_back(changeC[i]);
        }
    }

    string proSubCode(reEXP *re, int id);   //由最小化DFA生成正则程序代码

    void initCode(string inAddr, string outAddr);

    void outCPP(string addr);
};

/**     编译运行    **/
bool compileCode(string inAddr, string outAddr);    //编译代码

void runCode(string inAddr, string outAddr, string tny, string lex);    //运行代码

#endif //TEST01_GLOBAL_H
