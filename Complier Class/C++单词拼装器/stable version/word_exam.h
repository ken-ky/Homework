#ifndef UNTITLED_WORD_EXAM_H
#define UNTITLED_WORD_EXAM_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

void shown(vector<string> &lines);

void display_map(unordered_map<string, int> &words);

//1.生成相关数据
//读取关于保留字哈希表
void Read_txt(const string &url, unordered_map<string, int> &vec);

//读取样例代码文件
vector<string> Read_code(string url);

//2.判断字符串机制
// 单词种类的枚举数字、标识符、保留字、数字运算符、特殊符号、注释、字符串、位运算符、关系运算符、赋值运算符、条件运算符、其它运算符)
enum TokenID {
    NUMBER,
    NAME,
    KEYWORD,
    OPERA_NUM,
    SPECIAL,
    NOTE,
    STRING,
    OPERA_LOGIC,
    OPERA_BIT,
    OPERA_RELA,
    OPERA_ASSIGN,
    OPERA_CON,
    OPERA_OTHER
};

// 单词结构
struct TokenStru {
    TokenID ID;
    string val;
    string op;
    string word;

    void display();
    string GetInfo();
};

//生成保留字的哈希表
extern unordered_map<string, int> word_map;
extern vector<string> article;

//生成哈希表
void MakeMap();

//词级别分类
void GetVal(TokenStru &object, int &pos, string &buf);

int GetWord(TokenStru &object, int &pos, string &buf);

void GetOP(TokenStru &object, int &pos, string &buf, bool &row_mark);

void GetToken_normal(vector<TokenStru> &printer, string &buf, bool &row_mark);

//行级别分类
//宏定义语句分类
void GetToken_defined(vector<TokenStru> &printer, string &buf);

//注释语段分类
void GetToken_mark(vector<TokenStru> &printer, string &buf, bool &row_mark);

#endif //UNTITLED_WORD_EXAM_H
