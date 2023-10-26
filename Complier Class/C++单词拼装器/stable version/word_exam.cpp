#include "word_exam.h"

unordered_map<string, int> word_map;
vector<string> article;

void shown(vector<string> &lines) {
    for (auto line: lines) cout << line << endl;
}

void display_map(unordered_map<string, int> &words) {
    for (auto it = words.begin(); it != words.end(); it++) cout << "(" << it->first << "," << it->second << ")" << endl;
}

void MakeMap() {
    Read_txt("key_word.txt", word_map);
}

//1.生成相关数据
//读取关于保留字哈希表
void Read_txt(const string &url, unordered_map<string, int> &vec) {
    ifstream input_file(url);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '" << url << "'" << endl;
        return;
    }

    string temp;
    while (getline(input_file, temp)) {
//        cout << temp << endl;
        vec[temp]++;
    }
//    display_map(vec);
}

//读取样例代码文件
vector<string> Read_code(string url) {
    vector<string> vec;
    ifstream input_file(url);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '" << url << "'" << endl;
        return vec;
    }

    string temp;
    while (getline(input_file, temp)) {
//        cout << temp << endl;
        vec.emplace_back(temp);
    }
    return vec;
}

void TokenStru::display() {
    switch (ID) {
        case NUMBER:
            cout << "Number:" << val << endl;
            break;
        case NAME:
            cout << "Name:" << word << endl;
            break;
        case KEYWORD:
            cout << "Keyword:" << word << endl;
            break;
        case OPERA_NUM:
            cout << "Operator:" << op << endl;
            break;
        case OPERA_LOGIC:
            cout << "Logical operator:" << op << endl;
            break;
        case OPERA_ASSIGN:
            cout << "Assignment operator:" << op << endl;
            break;
        case OPERA_BIT:
            cout << "Bits operator:" << op << endl;
            break;
        case OPERA_RELA:
            cout << "Relation operator:" << op << endl;
            break;
        case OPERA_OTHER:
            cout << "Other operator:" << op << endl;
            break;
        case OPERA_CON:
            cout << "Conditional operator:" << op << endl;
            break;
        case SPECIAL:
            cout << "Special:" << word << endl;
            break;
        case NOTE:
            cout << "Note:" << word << endl;
            break;
        case STRING:
            cout << "String:" << word << endl;
            break;
        default:
            cerr << "Error! Type invalid." << endl;
            break;
    }
}

string TokenStru::GetInfo() {
    string s;
    switch (ID) {
        case NUMBER:
            s = "Number:";
            return s + val;
        case NAME:
            s = "Name:";
            return s + word;
        case KEYWORD:
            s = "Keyword:";
            return s + word;
        case SPECIAL:
            s = "Special:";
            return s + word;
        case NOTE:
            s = "Note:";
            return s + word;
        case STRING:
            s = "String:";
            return s + word;
        case OPERA_NUM:
            s = "Operator:";
            return s + op;
        case OPERA_LOGIC:
            s = "Logical operator:";
            return s + op;
        case OPERA_ASSIGN:
            s = "Assigment operator:";
            return s + op;
        case OPERA_BIT:
            s = "Bits operator:";
            return s + op;
        case OPERA_RELA:
            s = "Relation operator:";
            return s + op;
        case OPERA_OTHER:
            s = "Other operator:";
            return s + op;
        case OPERA_CON:
            s = "Conditional operator:";
            return s + op;
        default:
            cerr << "Error! Type invalid." << endl;
            return "Type invalid!";
    }
}

//词级别分类
void GetVal(TokenStru &object, int &pos, string &buf) {
    int begin = pos, len = 0;
    while ((buf[pos] >= '0' && buf[pos] <= '9') || (buf[pos] == 'x' || buf[pos] == 'X') ||
           (buf[pos] == 'b' || buf[pos] == 'B') || (buf[pos] == 'e' || buf[pos] == 'E') ||
           (buf[pos] == '.')) {
        len++;
        pos++;
    }
    object.ID = NUMBER;
    object.val = buf.substr(begin, len);
}

int GetWord(TokenStru &object, int &pos, string &buf) { //设置是否�?include"的输�?
    while (isalpha(buf[pos]) || isdigit(buf[pos]) || buf[pos] == '_') {
        object.word += buf[pos];
        pos++;
    }
//    cout << object.word << endl;

    //检测是否为保留字
    if (word_map.count(object.word)) {
        object.ID = KEYWORD;
        if (object.word == "include") return 1;
    } else object.ID = NAME;
    return 0;
}

void GetOP(TokenStru &object, int &pos, string &buf, bool &row_mark) {
    switch (buf[pos]) {
        case ',':
        case '.':
        case ';':
        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}': {
            object.ID = SPECIAL;
            object.word += buf[pos];
//			cout<<"debug:"<<buf[pos]<<endl;
            pos++;
            break;
        }
        case '/': {
            if (buf[pos + 1] == '/') {
                int begin = pos, len = 1;
                while (pos < buf.size()) {
                    pos++;
                    len++;
                }
                object.ID = NOTE;
                object.word = buf.substr(begin, len);
                pos++;
            } else if (buf[pos + 1] == '=') {
                object.ID = OPERA_ASSIGN;
                object.word = "/=";
                pos += 2;
            } else if (buf[pos + 1] == '*') {
                int begin = pos, len = 1;
                row_mark = true;
                while (pos < buf.size()) {
                    pos++;
                    len++;
                }
                object.ID = NOTE;
                object.word = buf.substr(begin, len);
                if (buf.substr(begin + len - 2, 2) == "*/") row_mark = false;
            } else {
                object.ID = OPERA_NUM;
                object.op += '/';
                pos++;
            }
            break;
        }
        case '*': {
            if (buf[pos + 1] == '=') {
                object.ID = OPERA_ASSIGN;
                object.word = "*=";
                pos += 2;
            } else {
                object.ID = OPERA_NUM;
                object.word = "*";
                pos++;
            }
            break;
        }
        case '<':
        case '>': {
            if (buf[pos + 1] == buf[pos] || buf[pos + 1] == '=') {
                if (buf[pos + 2] == '=') {
                    object.ID = OPERA_ASSIGN;
                    object.op = buf.substr(pos, 3);
                    pos += 3;
                } else if (buf[pos + 1] == '=') {
                    object.ID = OPERA_RELA;
                    object.op = buf.substr(pos, 2);
                    pos += 2;
                } else {
                    object.ID = OPERA_BIT;
                    object.op = buf.substr(pos, 2);
                    pos += 2;
                }
            } else {
                object.ID = OPERA_RELA;
                object.op += buf[pos];
                pos++;
            }
            break;
        }
        case '\'': {
            int len = 1, begin = pos;
            while (pos < buf.size()) {
                pos++;
                len++;
                if (buf[pos] == '\'') {
                    if (buf[pos - 1] == '\\') {
                        if (buf[pos - 2] == '\\') break;
                    } else break;
                }
            }
            object.ID = STRING;
            object.word = buf.substr(begin, len);
            pos = begin + len;
            break;
        }
        case '"': {
            int len = 1, begin = pos;
            while (pos < buf.size()) {
                pos++;
                len++;
                if (buf[pos] == '"') {
                    if (buf[pos - 1] == '\\') {
                        if (buf[pos - 2] == '\\') break;
                    } else break;
                }
            }
            object.ID = STRING;
            object.word = buf.substr(begin, len);
            pos = begin + len;
            break;
        }
        case '+':
        case '-':
        case '%':
            if (buf[pos + 1] == buf[pos]) {
                object.ID = OPERA_NUM;
                object.op = buf.substr(pos, 2);
                pos += 2;
            } else if (buf[pos + 1] == '=') {
                object.ID = OPERA_ASSIGN;
                object.op = buf.substr(pos, 2);
                pos += 2;
            } else if (buf[pos + 1] == '>') {
                object.ID = OPERA_OTHER;
                object.op = buf.substr(pos, 2);
                pos += 2;
            } else {
                object.ID = OPERA_NUM;
                object.op += buf[pos];
                pos++;
            }
            break;
        case '&':
        case '|':
        case '^':
        case '~':
            if (buf[pos + 1] == buf[pos]) {
                object.ID = OPERA_LOGIC;
                object.op = buf.substr(pos, 2);
                pos += 2;
            } else if (buf[pos + 1] == '=') {
                object.ID = OPERA_ASSIGN;
                object.op = buf.substr(pos, 2);
                pos += 2;
            } else {
                object.ID = OPERA_BIT;
                object.op += buf[pos];
                pos++;
            }
            break;
        case '=':
            if (buf[pos + 1] == buf[pos]) {
                object.ID = OPERA_RELA;
                object.op = buf.substr(pos, 2);
                pos += 2;
            } else {
                object.ID = OPERA_ASSIGN;
                object.op += buf[pos];
                pos++;
            }
            break;
        case '!':
            if (buf[pos + 1] == '=') {
                object.ID = OPERA_RELA;
                object.op = buf.substr(pos, 2);
                pos += 2;
            } else {
                object.ID = OPERA_LOGIC;
                object.op += buf[pos];
                pos++;
            }
            break;
        case '?':
        case ':':
            if (buf[pos + 1] == ':') {
                object.ID = SPECIAL;
                object.op = buf.substr(pos, 2);
                pos += 2;
            } else {
                object.ID = OPERA_CON;
                object.op += buf[pos];
                pos++;
            }
            break;
        default:
            object.ID = OPERA_OTHER;
            object.op += buf[pos];
            pos++;
    }
}

void GetToken_normal(vector<TokenStru> &printer, string &buf, bool &row_mark) {
    int pos = 0;
    while (pos < buf.size()) {
        //读到空格或者缩�?
        while (pos < buf.size() && (buf[pos] == ' ' || buf[pos] == '	')) pos++;  // 修改循环条件
        if (pos >= buf.size())break;
        TokenStru object;
        if ((buf[pos] >= '0') && (buf[pos] <= '9')) {     //数字处理
            GetVal(object, pos, buf);
        } else if (isalpha(buf[pos])) {
            GetWord(object, pos, buf);
        } else {
            GetOP(object, pos, buf, row_mark);
        }
//		object.display();
        printer.emplace_back(object);
    }
}

//行级别分�?
//宏定义语句分�?
void GetToken_defined(vector<TokenStru> &printer, string &buf) {
    int pos = 0, s, os_file = 0;
    while (pos < buf.size()) {
        //读到空格或者缩�?
        while (pos < buf.size() && (buf[pos] == 32 || buf[pos] == 9)) pos++;  // 修改循环条件
        TokenStru object;
        //针对"include"语句处理
        if (os_file && (buf[pos] == '<' || buf[pos] == '\"')) {
            int begin = pos, len = 1;
            while ((buf[pos] != '>' || buf[pos] != '\"') && pos < buf.size()) {
                len++;
                pos++;
            }
            object.ID = SPECIAL;
            object.word = buf.substr(begin, len + 1);
//            object.display();
        }
        if (buf[pos] == '#') {
            object.ID = SPECIAL;
            object.word += '#';
            pos++;
        } else if ((buf[pos] >= '0') && (buf[pos] <= '9')) {     //数字处理
            GetVal(object, pos, buf);
        } else if (isalpha(buf[pos])) {
//            cout<<"start:"<<buf[pos]<<endl;
            os_file = GetWord(object, pos, buf);
//            cout<<"end:"<<os_file<<endl;
        } else pos++;
//        object.display();
        printer.emplace_back(object);
    }
}

void GetToken_mark(vector<TokenStru> &printer, string &buf, bool &row_mark) {
    int pos = 0, size = printer.size();
    while (pos < buf.size() && (buf[pos] == 32 || buf[pos] == 9)) pos++;
    int begin = pos, len = 0;
    while (pos < buf.size() - 1) {
        if (buf[pos] == '*' && buf[pos + 1] == '/') {
            len++;
            row_mark = false;
            printer[size - 1].word += buf.substr(begin, len + 1);
            return;
        }
        pos++;
        len++;
    }
    printer[size - 1].word += buf.substr(begin, len + 1);
}