### 实验一开发手册

---

##### 1. 编译环境

`IDE`环境：由`Jetbrains`团队开发的`Clion 2023.2.1`

`C++`环境：`工具链: MinGW_qt, MinGW`，`生成器：Ninja`（使用`CMake`）

编码环境：`全局编码：UTF-8，项目编码：GBK`

外部环境：`Qt 5.14.2`

<font color='red'>**【备注】**</font>：`Experiment03`为高亮版文件（未完成），`Experiment02`为高亮版文件



##### 2. 代码版本一 

1. 基本实现代码对于数字、符号、标识符识别
2. 将语句分为**宏定义语句**、**多行注释语句**和**普通代码语句**，从而得到三个函数识别语句：`GetToken_defined`、`GetToken_mark`、`GetToken_normal`。
3. 确定了从外部读入`txt`文件初始化关键字表的方法，方便及时更新选择规则。



###### (1) 数字识别函数

```c++
//词级别分类
void GetVal(int &s, TokenStru &object, int &pos, string &buf) {
    int begin = pos;
    s = 0;
    while ((buf[pos] >= '0' && buf[pos] <= '9') || (buf[pos] == 'x' || buf[pos] == 'X') ||
           (buf[pos] == 'b' || buf[pos] == 'B') || (buf[pos] == 'e' || buf[pos] == 'E')) {
        s = s * 10 + buf[pos] - '0';
        pos++;
    }
    object.ID = NUMBER;
    object.val = s;

    if (buf[pos] == '.') {
        int len = 0;
        s = 0;
        pos++;
        while (buf[pos] >= '0' && buf[pos] <= '9') {
            s = s * 10 + buf[pos] - '0';
            len++;
            pos++;
        }
        object.val = object.val + s / pow(10, len);
    }
    if (buf[begin - 1] == '-') s = 0 - s;   //设置负数情况
}
```



###### (2) 符号识别函数

```c++
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
            object.ID = STRING;
            object.word = buf.substr(begin, len);
            pos = begin + len - 1;
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
            object.ID=OPERA_OTHER;
            object.op+=buf[pos];
            pos++;
    }
}
```



###### (3) 标识符识别函数

```c++
int GetWord(TokenStru &object, int &pos, string &buf) {
    //设置是否include"的输入
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
```



###### (4) 多行注释识别函数

此函数与**符号识别函数**一起对多行注释进行识别处理

```c++
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
```



###### (5) <font color="Orange">迭代的确定方向</font>

1. 完成多文件结构的划分
2. 完成图形化程序书写



##### 3. 代码版本二 

1. 实现了功能的图形化界面
2. 对部分代码进行了改进，修复了数字检测的bug，拓宽了数字表达的格式
3. 细化了符号检测的种类，符号分类更加多样化
4. 将原本终端显示函数替换为了在qt页面显示



###### (1) 数字检测函数

```c++
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
```



###### (2) 符号分类函数

```c++
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
            pos = begin + len - 1;
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
```



###### (3) 字符串化结构函数

```c++
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
```





###### (4) 窗口显示函数

```c++
void WordsAssembler::ShowResult() {
    string s;
    for (auto re: res) s += re.GetInfo() + "\n";
    outputTextEdit->setText(QString::fromStdString(s));
    outputTextEdit->show();
}
```



###### (5) 版本总结

+ 当前版本特点：
  1. 沿用了之前的结构体`TokenStru`来存储单个词（数字、符号、字符等）的检测结果，使用`vector<TokenStru>`来储存所有的检测结果
  2. 使用`GetInfo`来得到`TokenStru`对应的`string`字符串，通过这种方式来显示检测的结果
  3. 使用了多文件结构进行项目构建（包括数据文件），具体关系见下表：

|        文件名         |                       代码功能                       |
| :-------------------: | :--------------------------------------------------: |
|   `CMakeLists.txt`    | `Clion`关联文件的`CMake`工具链方式，用于确定配置方式 |
| `ui_WordsAssembler.h` |      图形化基本结构头文件（对选择文件框架构建）      |
|  `WordsAssembler.ui`  |             对应`Qt`文件构建的图形化框架             |
|     `ui_frame.h`      | `Qt`主窗口的构建头文件，用于声明元件、信号、槽函数等 |
|    `ui_frame.cpp`     |    对上述头文件槽函数函数的定义（文件选择和显示）    |
|      `frame.ui`       |          对应上述`C++`文件的`Qt`图形化框架           |
|     `word_exam.h`     |      词法分析文件源程序的头文件，声明使用的函数      |
|    `word_exam.cpp`    |               对上述主要函数的定义补充               |
|      `main.cpp`       |            调用主函数构造全局的函数源文件            |
|    `key_word.txt`     |           存储函数关键字对应规则的数据文件           |
|      `Test.cpp`       |                用于代码识别的测试文件                |



+ 迭代版本方案：

  1. 完成高亮`html`显示
  2. 将`TokenStru`的结构直接变为`string`类型（因为高亮以颜色划分类型 ，不需要文本进行显示了）

+ 未解决的问题：

  【中文编码】`Qt`中文路径下构建错误，此问题会导致`.exe`文件无法置于中文路径下工作



##### 4. 代码版本三（未完成）

###### (1) 高亮化代码构建

```c++
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
```



###### (2) 图形界面显示函数

```c++
void WordsAssembler::ShowResult() {
    string s;
    for (auto re: res) s += re;
    outputTextEdit->setText(QString::fromStdString(s));
    outputTextEdit->show();
    cout << s << endl;
    res.clear();
}
```



###### (3) 版本总结

+ 版本特点：
  1. 将存储结构`TokenStru`完全改写为`string`
  2. 图形化显示高亮字体，显示的对应关系见下表

|    类型    |                      颜色                      |    意义    |
| :--------: | :--------------------------------------------: | :--------: |
|   `Num`    |         <font color="Blue">Blue</font>         |    数字    |
| `Keyword`  | <font color='MediumOrchid'>MediumOrchid</font> |   关键字   |
| `Special`  |   <font color='Aquamarine'>Aquamarine</font>   |  特殊符号  |
|   `Note`   |         <font color='Lime'>Lime</font>         |    注释    |
| `AssignOp` |    <font color='LimeGreen'>LimeGreen</font>    | 赋值运算符 |
|  `NumOp`   |   <font color='BlueViolet'>BlueViolet</font>   | 数字运算符 |
|  `RelaOp`  |   <font color='DarkOrange'>DarkOrange</font>   | 关系运算符 |
|  `BitOp`   |         <font color='Gold'>Gold</font>         |  位运算符  |
|  `StrOp`   |    <font color='LawnGreen'>LawnGreen</font>    |   字符串   |
|  `OthOp`   |    <font color='OliveDrab'>OliveDrab</font>    | 其它运算符 |
|  `LogOp`   |    <font color='PaleGreen'>PaleGreen</font>    | 逻辑运算符 |
|  `ConOp`   |         <font color='Peru'>Peru</font>         | 条件运算符 |

+ 未解决问题：
  1. “<”、“>”造成的`html`显示问题
  2. 空格与缩进需要重排
  3. 中文路径无法解析
