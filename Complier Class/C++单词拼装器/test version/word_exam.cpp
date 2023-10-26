# include "word_exam.h"

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

//词级别分类
void GetVal(string &object, int &pos, string &buf) {
    int begin = pos, len = 0;
    while ((buf[pos] >= '0' && buf[pos] <= '9') || (buf[pos] == 'x' || buf[pos] == 'X') ||
           (buf[pos] == 'b' || buf[pos] == 'B') || (buf[pos] == 'e' || buf[pos] == 'E') ||
           (buf[pos] == '.')) {
        len++;
        pos++;
    }
    object += "<span style=\"color:Blue\">" + buf.substr(begin, len) + "</span>";
}

int GetWord(string &object, int &pos, string &buf) { //设置是否�?include"的输�?
    int begin = pos, len = 0;
    while (isalpha(buf[pos]) || isdigit(buf[pos]) || buf[pos] == '_') {
        len++;
        pos++;
    }
    string temp = buf.substr(begin, len);

    //检测是否为保留字
    if (word_map.count(temp)) {
        if (temp == "include") {
            object += "<span style=\"color:MediumOrchid\">include</span>";
            return 1;
        }
        object += "<span style=\"color:MediumOrchid\">";
    } else {
        object += "<span style=\"color:MediumBlue \">";
    }
    object += temp + "</span>";
    return 0;
}

void GetOP(string &object, int &pos, string &buf, bool &row_mark) {
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
            object += "<span style=\"color:Aquamarine\">" + string(1, buf[pos]) + "</span>";
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
                object += "<span style=\"color:Lime\">" + buf.substr(begin, len) + "</span>";
                pos++;
            } else if (buf[pos + 1] == '=') {
                object += "<span style=\"color:LimeGreen\">/=</span>";
                pos += 2;
            } else if (buf[pos + 1] == '*') {
                int begin = pos, len = 1;
                row_mark = true;
                while (pos < buf.size()) {
                    pos++;
                    len++;
                }
                object += "<span style=\"color:Lime\">" + buf.substr(begin, len);
                if (buf.substr(begin + len - 2, 2) == "*/") {
                    row_mark = false;
                    object += "</span>";
                }
            } else {
                object += "<span style=\"color:BlueViolet\">/</span>";
                pos++;
            }
            break;
        }
        case '*': {
            if (buf[pos + 1] == '=') {
                object += "<span style=\"color:LimeGreen\">*=</span>";
                pos += 2;
            } else {
                object += "<span style=\"color:BlueViolet\">*</span>";
                pos++;
            }
            break;
        }
        case '<':
        case '>': {
            if (buf[pos + 1] == buf[pos] || buf[pos + 1] == '=') {
                if (buf[pos + 2] == '=') {
                    object += "<span style=\"color:LimeGreen\">" + buf.substr(pos, 3) + "</span>";
                    pos += 3;
                } else if (buf[pos + 1] == '=') {
                    object += "<span style=\"color:DarkOrange\">" + buf.substr(pos, 2) + "</span>";
                    pos += 2;
                } else {
                    object += "<span style=\"color:Gold\">" + buf.substr(pos, 2) + "</span>";
                    pos += 2;
                }
            } else {
                object += "<span style=\"color:DarkOrange\">" + string(1, buf[pos]) + "</span>";
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
            object += "<span style=\"color:LawnGreen\">" + buf.substr(begin, len) + "</span>";
            pos = begin + len - 1;
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
            object += "<span style=\"color:LawnGreen\">" + buf.substr(begin, len) + "</span>";
            pos = begin + len;
            break;
        }
        case '+':
        case '-':
        case '%':
            if (buf[pos + 1] == buf[pos]) {
                object += "<span style=\"color:Blue\">" + buf.substr(pos, 2) + "</span>";
                pos += 2;
            } else if (buf[pos + 1] == '=') {
                object += "<span style=\"color:LimeGreen\">" + buf.substr(pos, 2) + "</span>";
                pos += 2;
            } else if (buf[pos + 1] == '>') {
                object += "<span style=\"color:OliveDrab\">" + buf.substr(pos, 2) + "</span>";
                pos += 2;
            } else {
                object += "<span style=\"color:OliveDrab\">" + string(1, buf[pos]) + "</span>";
                pos++;
            }
            break;
        case '&':
        case '|':
        case '^':
        case '~':
            if (buf[pos + 1] == buf[pos]) {
                object += "<span style=\"color:PaleGreen\">" + buf.substr(pos, 2) + "</span>";
                pos += 2;
            } else if (buf[pos + 1] == '=') {
                object += "<span style=\"color:LimeGreen\">" + buf.substr(pos, 2) + "</span>";
                pos += 2;
            } else {
                object += "<span style=\"color:Gold\">" + string(1, buf[pos]) + "</span>";
                pos++;
            }
            break;
        case '=':
            if (buf[pos + 1] == buf[pos]) {
                object += "<span style=\"color:DarkOrange\">" + buf.substr(pos, 2) + "</span>";
                pos += 2;
            } else {
                object += "<span style=\"color:LimeGreen\">" + string(1, buf[pos]) + "</span>";
                pos++;
            }
            break;
        case '!':
            if (buf[pos + 1] == '=') {
                object += "<span style=\"color:DarkOrange\">" + buf.substr(pos, 2) + "</span>";
                pos += 2;
            } else {
                object += "<span style=\"color:PaleGreen\">" + string(1, buf[pos]) + "</span>";
                pos++;
            }
            break;
        case '?':
        case ':':
            if (buf[pos + 1] == ':') {
                object += "<span style=\"color:Aquamarine\">" + buf.substr(pos, 2) + "</span>";
                pos += 2;
            } else {
                object += "<span style=\"color:Peru\">" + string(1, buf[pos]) + "</span>";
                pos++;
            }
            break;
        default:
            object += "<span style=\"color:OliveDrab\">" + string(1, buf[pos]) + "</span>";
            pos++;
    }
}

void GetToken_normal(vector<string> &printer, string &buf, bool &row_mark) {
    int pos = 0;
    while (pos < buf.size()) {
        //读到空格或者缩�?
        while (pos < buf.size() && (buf[pos] == ' ' || buf[pos] == '	')) pos++;  // 修改循环条件
        if (pos >= buf.size())break;
        string object;
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
void GetToken_defined(vector<string> &printer, string &buf) {
    int pos = 0, s, os_file = 0;
    while (pos < buf.size()) {
        //读到空格或者缩�?
        while (pos < buf.size() && (buf[pos] == 32 || buf[pos] == 9)) pos++;  // 修改循环条件
        string object;
        //针对"include"语句处理
        if (os_file && (buf[pos] == '<' || buf[pos] == '\"')) {
            int begin = pos, len = 1;
            while ((buf[pos] != '>' || buf[pos] != '\"') && pos < buf.size()) {
                len++;
                pos++;
            }
            object += "<span style=\"color:Aquamarine\">" + buf.substr(begin, len + 1) + "</span>";
//            object.display();
        }
        if (buf[pos] == '#') {
            object += "<span style=\"color:Aquamarine\">#</span>";
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

void GetToken_mark(vector<string> &printer, string &buf, bool &row_mark) {
    int pos = 0, size = printer.size();
    while (pos < buf.size() && (buf[pos] == 32 || buf[pos] == 9)) pos++;
    int begin = pos, len = 0;
    while (pos < buf.size() - 1) {
        if (buf[pos] == '*' && buf[pos + 1] == '/') {
            len++;
            row_mark = false;
            printer[size - 1] += buf.substr(begin, len + 1) + "</span></div>";
            return;
        }
        pos++;
        len++;
    }
    printer[size - 1] += buf.substr(begin, len + 1);
}